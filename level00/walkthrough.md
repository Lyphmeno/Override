level00
=======

```console
RELRO           STACK CANARY      NX            PIE             RPATH      RUNPATH      FILE
Partial RELRO   No canary found   NX enabled    No PIE          No RPATH   No RUNPATH   /home/users/level00/level00
```

*	We spawn with a [level00](src/level00) executable.
*	Here is the [code](src/level00.c):
	```c
	if (password == 0x149c)
			system("/bin/sh");
	```
	Converting `0x149c` to decimal gives `5276`.
*	Not much to talk about here, just a classic number check then spawning a shell :
	```console
	level00@OverRide:~$ ./level00 
	***********************************
	*            -Level00 -           *
	***********************************
	Password:5276

	Authenticated!
	$ whoami
	level01
	$ cat /home/users/level01/.pass
	uSq2ehEGT6c9S24zbshexZQBXUGrncxn5sD5QfGL
	```