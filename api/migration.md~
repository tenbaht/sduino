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

|C++ name				| C name		|
|--------				| -------		|
|`Serial.print(int)`			| `Serial_print_i`	|
|`Serial.print(unsigned)`		| `Serial_print_u`	|
|`Serial.print(float)`			| `Serial_print_f`	|
|`Serial.print(char)`			| `Serial_print_c`	|
|`Serial.print(char *)`			| `Serial_print_s`	|
|`Serial.print(char *buf, int len)`	| `Serial_print_n`	|
|`Serial.print(unsigned n, int base)`	| `Serial_print_ub`	|
|`random(long)`				| `random`		|
|`random(long howsmall, long howbig)`	| `random_minmax`	|



### The _Generic selection

The C11 standard introduced the
[`_Generic`](http://en.cppreference.com/w/c/language/generic)
selection function. This function allows for automatic selection of
different function variants at compile time depending on the type of the
function arguments mimicing polymorph C++ functions.

Recent versions of SDCC already support this function (command line argument
`--std-sdcc99`), but it's usefulness is still limited to some special cases.

```c
#define Serial_print(X) _Generic((X), \
    char*: Serial_print_s, \
    signed long: Serial_print_i, \
    signed int: Serial_print_i, \
    signed char: Serial_print_c, \
    unsigned long: Serial_print_u, \
    unsigned int: Serial_print_u, \
    unsigned char: Serial_print_u \
    )(X)
```

This would unify some, but not all print function variants:

|C++ name				| C name using _Generic	|
|--------				| ------------------	|
|`Serial.print(int)`			| `Serial_print`	|
|`Serial.print(unsigned)`		| `Serial_print`	|
|`Serial.print(char)`			| `Serial_print`	|
|`Serial.print(char *)`			| `Serial_print_s`	|
|`Serial.print(char *buf, int len)`	| `Serial_print_n`	|
|`Serial.print(unsigned n, int base)`	| `Serial_print_ub`	|

Unfortunately cpp does not match string constants and `char*` resulting in a
very non-regular usage pattern:

	char *string="Hello";

	Serial_print(string);	// works
	Serial_print("Hello");	// doesn't work
	Serial_print_s("Hello");// works

To avoid too much confusion it might be better to not use `_Generic` at all.

Another problem using the `_Generic` selector is configurable instance
names. The preprocessor does not allow for variable macro names. That means
`_Generic` would work with fixed name like `Serial`, but it wouldn't work
for `SoftwareSerial` with no standard instance name.




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

If multiple instances need to be supported, an approach similar to a C file
descriptor is used. The "constructor" function allocates and initializes a
data structure for one particular instance and return a pointer to this
structure. Typically, this is done in the `setup()`-function and this pointer 
is stored in a global variable to be used as a device descriptor.


So far the [Stepper library](api/Stepper.md) is the only example:

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

Pin mode		|Pin properties
---------------------	|------------------------------------
`OUTPUT`		|output, push-pull, slow mode (default)
`OUTPUT_OD`		|output, open drain, fast mode
`OUTPUT_FAST`		|output, push-pull, fast mode
`OUTPUT_OD_FAST`	|output, open drain, fast mode


### Timer

`millis()` uses timer4. The prescaler and end value is calculated at compile
time for a cycle time as close to 1ms as possible. Default values @16Mhz:
prescaler=64, counter cycle=250 (end value=249), resulting in exactly 1ms
intervals.

timer1: PWM for PC3, PC4, on alternate mapping PC6, PC7, could be used for ADC  
timer2: PWM for PA3, PD3, PD4 or PC5 (mutual exclusive)  
timer4: millis()  


### Leaving out unused parts

Some functions of the core Arduino system can be left out on compilation to
save code space. This is done by compiler flags that can be defined in the
Makefile:

```make
BOARD_TAG = stm8sblue
CFLAGS = -DNO_SERIAL -DNO_ANALOG_IN -DNO_ANALOG_OUT

include ../../../sduino.mk
```

These flags are supported:

Flag		| Bytes saved	| Functions lost
-----		| -------:	| ---------
NO_SERIAL	| 765		| all serial communication
NO_ANALOG_OUT	| 406		| analogWrite()
NO_ANALOG_IN	| 56		| analogRead()



### Other modifications

`makeWord(unsigned char, unsigned char)` is an inline function now.


### Additional compile-time flags

Some internal details can be influenced by setting compile-time defines
using the `CFLAGS=-Dflagname` line in the Makefile.

**`SUPPORT_ALTERNATE_MAPPINGS`**:
Allow the use of `alternateFunctions()`

**`ENABLE_SWIM`**:
Do not disable the remote debugging function on the SWIM pin. This means
that this pin can not be used for normal I/O functions.

**`USE_SPL`**:
Use SPL functions for I/O access instead of direct register accesses. Useful
only for debugging and porting to other CPU variants. Do not use for regular
development.




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

**Input-Capture-Mode:** Available for all four channels, at least for
timer1. Would be great for precise time measurements. Maybe build a library?

**Encoder interface mode:** Hardware support for reading quadrature encoder
and keeping track of the current (motor) position using a hardware timer
counter. Perfectly suited to all kinds of position feedback.
