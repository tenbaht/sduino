# Limitations

## Compatibility with the Arduino world

I adopted the Arduino core functionality for the STM8S to set up a simple
programming environment. But unfortunatly there is no free C++ compiler for
these CPUs. This makes it impossible to do a full port of the whole
enviroment and integrate it 100% in the Arduino world as it has been done
for the STM32 and the ESP8266.

This is not a drop-in replacement for an AVR, but the programming API is
still very, very similar. Adopting existing libraries from C++ to C for use
with the simplified C API is often easy and can be done quite fast,
depending on the degree of dependency on specific hardware features.

Using a converter/compiler like
[cfront](https://en.wikipedia.org/wiki/Cfront) to translate from C++ to C
might be an option.


## Float arithmetics

Floating point arithmetics is supported by the SDCC standard library, but it
comes at a pretty high cost in terms of code space and CPU load. This is how
much the generated code grows by using a single float operation compared to 
using a long int:

Floating point operation	|approx. code size
--------------------		|---------
addition			| 736 Bytes
subtraction			| 754 Bytes
division			| 673 Bytes
multiplication			| 907 Bytes
sinf() or cosf()		| 3346 Bytes
log10f()			| 3437 Bytes

The Arduino standard example '01. Basics/ReadAnalogVoltage' is a very simple
simple program with only very little floating point arithmetics. But it
already uses 7336 bytes of flash. A similar sketch using integer arithmetics
results in much more compact code occuping only 3791 bytes.

Float does work, but is better to be avoided and replaced by fixed point
arithmetics whenever possible.



## Known problems using Windows

It works, but using the Arduino.mk makefile with Windows is slow. **Very**
slow. Painfully slow. Compiling-the-Blink-example-takes-about-40-seconds
kind of slow. Yes, seriously. No kidding. 40 seconds. Measured on a 3GHz
machine with 4GB RAM.

There is no easy fix, the underlying problem is a fundamental one. It is not
about the compilation itself, it is the way Makefiles are written and
executed. The whole concept relies on forking subprocesses for all the shell
calls. Unfortunately, there is nothing like a fork in Windows and to work
around that is painfully slow.

It would be great if somebody would replace the painfully slow parameter
checking part of the makefile (that causes the majority of forking) by a
single shell script that gets called by the makefile and delivers the
results in no time. Or use [cmake](https://cmake.org). Or integrate it somehow
into the STVD IDE (This [STVD-SDCC integration
suite](https://github.com/shkolnick-kun/stvd-sdcc) might be useful). Or
whatever.

Until then the least annoying way out might be using a virtual machine
running a simple Linux system. Ubuntu Mate or a basic Debian install for
example. Virtual Box is great for this purpose and freely available. The new
Linux Subsystem feature of Windows 10 might be another option.

