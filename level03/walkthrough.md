 level03
=======
```
RELRO           STACK CANARY      NX            PIE             RPATH      RUNPATH      FILE
Partial RELRO   Canary found      NX enabled    No PIE          No RPATH   No RUNPATH   /home/users/level03/level03
```

*	We spawn with a *level03* executable and here is the [code](src/level03.c)
* Looking at the code, we notice:
  - `main()` calls `test()` with our input and `322424845`.
  - `test()` calculates `322424845 - input` and passes the result to `decrypt()` if it matches one of the accepted cases.
  - `decrypt()` XORs **Q}|u`sfg~sf{}|a3** with the key.
  - If the result equals `"Congratulations!"`, the program executes `system("/bin/sh")`.
* We need to find the correct XOR key. Since we know the expected string, we can XOR the first encrypted character with the first expected character:
  ```text
  'Q' ^ 'C' = 0x12 = 18
  ```
* We can confirm this with our [decrypt tool](src/encrypt.c):
  ```console
  Key: 18 (0x12)
  Decrypted: Congratulations!
  ```
* Now we need `test()` to calculate `18`:
  ```text
  322424845 - input = 18
  input = 322424827
  ```
* Let's try it:
  ```console
  level03@OverRide:~$ ./level03
  ***********************************
  *               level03         **
  ***********************************
  Password:322424827
  $ whoami
  level04
  $ cd ../level04
  $ cat .pass
  kgv3tkEb9h2mLkRsPkXRfc2mHbjMxQzvb2FrgKkf
  ```