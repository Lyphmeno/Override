level06
=======
```
RELRO           STACK CANARY      NX            PIE             RPATH      RUNPATH      FILE
Partial RELRO   Canary found      NX enabled    No PIE          No RPATH   No RUNPATH   /home/users/level06/level06
```

*	We have the *level06* executable and here is the [code](src/level06.c)
* Looking at the code, we notice:
  - The program asks for a `login` and a `serial`.
  - The login must contain at least 6 characters.
  - `ptrace(PTRACE_TRACEME)` is used as an anti-debugging check.
  - The serial is calculated from the login inside `auth()`.
  - If the serial is correct, the program executes `system("/bin/sh")`.
* We can simply reproduce the serial calculation instead of exploiting a memory vulnerability:
  ```c
  int expected = (login[3] ^ 0x1337) + 6221293;

  for (int i = 0; i < length; i++)
      expected += (expected ^ (unsigned int)login[i]) % 0x539;
  ```
  - The initial value uses the 4th character of the login, XORed with `0x1337`.
  - For each character, the current serial is XORed with its ASCII value.
  - The result is reduced modulo `0x539` (1337) and added to the serial.
* We can reverse this process by reproducing the algorithm in our [solution](src/solution.c), using `aaaaaa` as our login:
  ```console
  $ gcc solution.c -o solution
  $ ./solution
  Serial: 6231562
  ```
* Let's try it:
  ```console
  level06@OverRide:~$ ./level06
  ***********************************
  *               level06           *
  ***********************************
  -> Enter Login: aaaaaa
  ***********************************
  ***** NEW ACCOUNT DETECTED ********
  ***********************************
  -> Enter Serial: 6231562
  Authenticated!
  $ whoami
  level07
  $ cd ../level07
  $ cat .pass
  GbcPDRgsFK77LNnnuh7QyFYA2942Gp8yKj9KrWD8
  ```