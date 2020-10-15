# FAQ/common problems

## Compilation issues

### ASlink-Warning-No definition of area SSEG

This happens when there is no *.pde or *.ino file in the project directory,
`Serial_begin()` is not used and only *.c files are compiled.

This message means that main.c is not pulled in by the linker because there
was no reference to main() anywhere. When processing *.pde/ino files
`wrapper/sdcc.sh` (for IDE builds) or `Arduino.mk` (for Makefile builds) adds
a reference to main:

```
/* add a dummy reference to main() to make sure to pull in main.c from the core library */
void main(void);
void (*dummy_variable) () = main;
```

If there is no pde/ino file the user has to make sure main.c is pulled in by
the linker or define its own main().

Possible ways to pull in main.c:

- Use Serial_begin(): This references the variable runSerialEvent which in turn pulls in main.c (some overhead)
- reference the variable runSerialEvent yourself: `begin(){runSerialEvent=0;}` (Overhead: 4 bytes flash)
- add a reference to main() like above. (Overhead: 2 bytes RAM and 2 bytes flash)
- define your own main() function.


### Flashing fails on the new board

It might be locked/write protected. [Check
this](../../hardware/stm8blue/#unlocking-a-write-protected-mcu)


## Hardware issues


### My new stm8blue board seems dead and does not respond to the flash tool

***Crap alert:*** There are [known quality
issues](../../hardware/stm8blue/#crap-alert) with some more recent PCB
production lots of the stm8blue boards.
