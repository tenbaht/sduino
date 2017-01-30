# Migrating existing code from C++ to C syntax

The original Arduino environment uses C++ syntax while sduino can only use
plain C syntax. Luckily, not many C++ features are used and in most cases a
conversion is not very hard. In most cases a conversion from C++ to C it is
just a matter of exchanging a dot for an underscore:

A C++ method name `class.method()` becomes a C function name `class_method()`.

This is possible since most libraries are written to be used as a singleton
anyway, so the fixed name prefix is not a problem. Only very few libraries
need a slightly more complex approach to be able to deal with multiple
instances.

There are two bigger problems left:



## Polymorph functions

The concept of polymorphism does not exist for plain C. As a workaround
'mangled' function names are used for the different parameter type
combinations supported by the original polymorph methods.

Typical name extensions are: `_u` for unsigned values, `_i` for signed
integer values, `_c`for characters, `_s` for strings, `_n` for data
buffer/length combinations.

For more non-regular polymorphism the name extension is often related to the
different use cases or to the names of the given parameters. Refer to the
respective library header file for details.

Some examples of typical name changes:

C++ name				| C name
-------					| -------
`Print.print(int)`			| `Print_print_i`
`Print.print(unsigned)`			| `Print_print_u`
`Print.print(char)`			| `Print_print_c`
`Print.print(char *)`			| `Print_print_s`
`Print.print(char *buf, int len)`	| `Print_print_n`
`Print.print(unsigned n, int base)`	| `Print_print_ub`
`random(long)`				| `random`
`random(long howsmall, long howbig)`	| `random_minmax`




## Inheritance from Print class

Most character output modules inherit methods from the Print class by
providing a virtual write method. A similar result can be achived by
providing a function pointer to the write function to be used to the print
functions.

This additional parameter is hidden from the user by providing more
convinient defines in every library that need to 'inherit' functions from
Print. This way `lcd.print("Hello World!")` becomes `lcd_print_s("Hello
World!")` and `Serial.print("Hello World!")` becomes `Serial_print_s("Hello
World!")`. Both call the same code from Print, but with different function
pointers to their own putchar/write function.



## Libraries with multiple instances

If multiple instances need to be supported, an approach similar to C file
descriptors is used. The "constructor" function allocates and initializes a
data structure for one particular instance and return a pointer to this
structure. Typically, this is done in the `setup()`-function and this pointer 
is stored in a global variable to be used as a device descriptor.


So far the [Stepper library](Stepper.md) is the only example:

```c
#include <Stepper.h>
Stepper myStepper;		// variable to store the "device descriptor"
void setup(void) {
	myStepper = Stepper_2phase(100,11,12);	// init a 2 phase stepper
}
void loop() {
	Stepper_step(myStepper, 20);		// do 20 steps forward
}
```


## Differences from the original Arduino environment

### Additional output pin modes

|Pin mode		|Pin properties
|---------------------	|------------------------------------
|`OUTPUT`		|output, push-pull, slow mode (default)  
|`OUTPUT_OD`		|output, open drain, fast mode  
|`OUTPUT_FAST`		|output, push-pull, fast mode  
|`OUTPUT_OD_FAST`	|output, open drain, fast mode  


### Timer

`millis()` uses timer4. The prescaler and end value is calculated at compile
time for a cycle time as close to 1ms as possible. Default values @16Mhz:
prescaler=64, counter cycle=250 (end value=249), resulting in exactly 1ms
intervals.

timer1: PWM for PC6, PC7 (8,9), could be used for ADC  
timer2: PWM for PA3 (2)  
timer4: millis()  


### Other modifications

`makeWord(unsigned char, unsigned char)` is an inline function now.


### Additional compile-time flags

Some internal details can be influenced by setting compile-time defines
using the `CFLAGS=-Dflagname` line in the Makefile.

Compile-time flag		| Purpose
-----------------		| --------
`SUPPORT_ALTERNATE_MAPPINGS`	| Allow the use of `alternateFunctions()`
`ENABLE_SWIM`			| Do not disable the remote debugging
function on the SWIM pin. This means that this pin can not be used for
normal I/O functions.
`USE_SPL`			| Use SPL functions for I/O access instead
of direct register accesses. Useful only for debugging and porting to other
CPU variants. Do not use for regular development.
``				|
``				|
``				|



## General notes on the Arduino port


### ADC
the prescaler is initialised for an ADC clock in the range of 1..2 MHz. The
minimum prescaler value is 2, so for a clock speed of less than 2 MHz the
required minimum ADC clock frequency can not be reached anymore.


### Mapping of logical pin numbers to register addresses
The lookup-table approach for assigning port and bit adresses to the logical
pin numbers is not effient on the STM8. The hole system could be changed to
a more regular scheme and replace the tables by hardcoded adress
calculations.


### Inefficient compilation
`digitalWrite` compiles very ineffiently. It might be worth some hand
optimization.


### Accessing the alternate pin functions
Added `alternateFunction()` to allow switching of some pins to their alternate
functions. This allows for three more PWM pins, but maybe it adds to much
complexity for the Arduino API. Not sure if it should stay. Has to be
enabled by defining `SUPPORT_ALTERNATE_MAPPINGS`.


### Useful CPU features that are not supported by the Arduino API

**Input-Capture-Mode:** Available for all four channels, at least for timer1. Would be great for precise time measurements. Maybe build a library?

**Encoder interface mode:** Kann von Haus aus mit Quadratur-Encodern umgehen
und in Hardware zählen -> perfekt für die Druckerschlitten-Motorsteuerung.

