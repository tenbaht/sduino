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

