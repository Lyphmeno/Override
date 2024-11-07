level03
=======
```
RELRO           STACK CANARY      NX            PIE             RPATH      RUNPATH      FILE
Partial RELRO   Canary found      NX enabled    No PIE          No RPATH   No RUNPATH   /home/users/level03/level03
```
*	Here we spawn with [level03](src/level03) executable
*	Looking at the [code](src/level03.c) gives us everything we need to finish this one
	- `test()` is launch with `322424845` as second parameter
	- `/bin/sh` is executed in `decrypt()` if the `v4` is equal to `Congratulations!`
	- `v4` is encrypted depending on a random value at compilation
	- all we have to do would be to find which `case` is the right encryption number
*	To do so we can either test every cases or try to reverse the encryption. For the sake of logic we made a [program](src/encrypt.c) in c to reverse the encryption
```console
lyphmeno@Mnemosyne:~/42/Ghub/Override/level03/src$ ./a.out 
	Decrypted password with ctx = 0x00: Q}|u`sfg~sf{}|a3
	Decrypted password with ctx = 0x05: Txypevcb{vc~xyd6
	Decrypted password with ctx = 0x06: W{zsfu`axu`}{zg5
	Decrypted password with ctx = 0x07: Vz{rgta`yta|z{f4
	Decrypted password with ctx = 0x08: Yut}h{nov{nsuti;
	Decrypted password with ctx = 0x09: Xtu|izonwzortuh:
	Decrypted password with ctx = 0x0b: Zvw~kxmluxmpvwj8
	Decrypted password with ctx = 0x0d: \pqxm~kjs~kvpql>
	Decrypted password with ctx = 0x0e: _sr{n}hip}husro=
	Decrypted password with ctx = 0x0f: ^rszo|ihq|itrsn<
	Decrypted password with ctx = 0x10: Amlepcvwncvkmlq#
	Decrypted password with ctx = 0x11: @lmdqbwvobwjlmp"
--> Decrypted password with ctx = 0x12: Congratulations! <--
	Decrypted password with ctx = 0x13: Bnofs`utm`uhnor 
```
We found the encryption case --> `0x12(HEX) - 18(DEC)`
*	`test()` does a substraction between the second parameter and the argument we enter to determine which case will be used. All we have to do is make sure this substraction is equal to `18`
	- `322424845 - 18 = 32242482`
*	Let's try that :
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