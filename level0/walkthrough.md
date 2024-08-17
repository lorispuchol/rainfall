# level0
A binary is present. The program segfaults when executed without arguments and write `No !` on the stderror with any argument.

## Goal
Find the good argument which will execute a shell with the user `level1`.

## Install radare2

```bash
r2 level0
```

# Analyze the binary

analyze all (fcns + bbs) and autoname them
```bash
[0x08048de8]> aaa
```

List all functions
```bash
[0x08048de8]> afl
```

seek to main function
```bash
[0x08048de8]> s sym.main
```

disassemble function and pass to visual 
```bash
[0x08048ec0]> pdf
[0x08048ec0]> VV
```

We see that the program is calling atoi with the first argument of the program and comparing it to 0x1a7 (423 in decimal). If the comparison is true, the program will call execv with "/bin/sh" as argument,.

> __See the string "/bin/sh" in the binary__  
> $ gdb level0  
> (gdb) b *0x8048ee0  
> (gdb) run 423  
> (gdb) x/s 0x80c5348