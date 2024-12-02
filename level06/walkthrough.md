level06
=======
```
RELRO           STACK CANARY      NX            PIE             RPATH      RUNPATH      FILE
Partial RELRO   Canary found      NX enabled    No PIE          No RPATH   No RUNPATH   /home/users/level06/level06
```

*	We have the [level06](src/level06) executable and here is the [code](src/level06.c)
*	The code doesn't seem to be exploitable throught overflow or format string, on the other hand it is more an enigma:
	- the code asks for a `login`
	- then asks for a `pass`
	- looking at the code tells us the `pass` is the result of a function on the `login`
	- `login` must be at least 6 char
*	I just had to reverse the code [here](src/solution.c) with `aaaaaa` as login
	```console
	lyphmeno@Mnemosyne:~/42/Ghub/Override/level06/src$ ./a.out 
	pass = 6231562
	```
*	Now we should be able to finish the level:
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