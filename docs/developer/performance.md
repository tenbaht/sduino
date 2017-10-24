# Performance

The STM8 backend for SDCC has improved a lot since the release of version
3.6.0 in June 2016. But due to the very small number of registers the code
size will almost always be bigger than for the AVR. Depending on the exact
application the difference is anywhere between negligible and factor two.



## Comparing code size with the original AVR Arduino

Benchmarking the original Arduino examples from Arduino 1.0.5. The simple
Blinky compiles to 57 bytes of code, the total binary including the sduino
libraries is 1868 Bytes (0x74c).

So far, wiring_analog depends on wiring_digital, even when analogWrite is not
used. This could be solved by compiling the sduino functions separately into
a library.

|Name			|Code	|Total	|Linked files other than main and wiring
|---------------------	|------	|-----	|-----------------------------
|01. Basics/		|
|BareMinimum		|2	|1238	|-
|Blink			|57	|1870	|wiring_digital
|AnalogReadSerial	|205	|3452	|digital, analog, serial, print
|DigitalReadSerial	|57	|3160	|digital, serial, print
|Fade			|226	|2189	|digital, analog
|ReadAnalogVoltage	|	|	|float not yet implemented
|02. Digital/		|
|Debounce		|192	|2016	|digital


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
