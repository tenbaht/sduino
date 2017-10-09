# Stepper Library

A Library for driving stepper motors with 2, 4 or 5 phases.
Derived from the Arduino Stepper library v1.8.0.


## API

This library is a multi-instance library, it supports an arbitrary number of
stepper motors per sketch.

The API syntax is very similar to the original C++ syntax, thanks to some
[c preprocessor macro magic](../macro.md).

The declaration supports polymorphism, so all that is needed to adopt an
existing sketch for use with sduino is moving the opening bracket at
the class declarator line and replacing the dots in the method names for
underscores.


Arduino method			| sduino function
--------------			| ---------------
`Stepper myStepper(steps, pin1,pin2);`		| `Stepper (myStepper,steps,pin1,pin2);`
`Stepper myStepper(steps, pin1,pin2,pin3,pin4);`| `Stepper (myStepper,steps,pin1,pin2,pin3,pin4);`
`Stepper myStepper(steps, pin1,pin2,pin3,pin4,pin5);`| `Stepper (myStepper,steps,pin1,pin2,pin3,pin4,pin5);`
`myStepper.setSpeed(speed);`	| `myStepper_setSpeed(speed);`
`myStepper.step(n);`		| `myStepper_step(n);`
`myStepper.version();`		| `myStepper_version();`


### API additions

The sduino implementation adds some extra methods to the stock Stepper
library to allow to work arounds some limitations of C code vs. C++:

Arduino method			| sduino function
--------------			| ---------------
done automatically on start up	| `myStepper_activateOutputs();`
not impemented			| `myStepper_2phase(steps,pin1,pin2);`
not impemented			| `myStepper_4phase(steps,pin1,pin2,pin3,pin4);`
not impemented			| `myStepper_5phase(steps,pin1,pin2,pin3,pin4,pin5);`


#### Initializing the output pins on demand

In real C++ the internal class constructor method initializes the motor pins
to output mode automatically at start up of the programm. In C this doesn't
happen automatically. Most libraries implement a begin() method for exactly
this purpose, but this one does not include an explicit constructor method.

Instead, the output mode is set automatically right before the very first
motor step. This means that the driver pins stay uninitialized and are left
floating from the time of power up until the first step is requested. This
won't be a problem in most cases as most motor drivers are using pull-up or
pull-down resistors to ensure proper signal levels.

If needed, the application could call the added `activateOutputs()` method
in the setup() function to force an immediate port pin initialization:

```c
setup() {
  myStepper_activateOutputs();    // initializes the motor pins to output mode
}
```


#### Assigning the pin connections at run time

The way the instantiation macro works requires compile-time constant values
for all parameters (see below in section 'Implementation Details'). In order
to be able to (re-) configure the pin connections at run time this version
of the library implements the functions `2phase()`, `4phase()` and
`5phase()`. These functions act like a constructor and allow for full
re-initialisation of an already existing "object".

If you need to (re-) configure the pin connections at run time (e.g. read it
from a configuration EEPROM first) you can initialize a data structure with
`0` for the unknown pin numbers and set them later by calling the right
xphase()-function:

```c
Stepper (myStepper,0,0);

setup() {
	... figure out the pin numbers somehow ...
	myStepper_4phase(stepsPerRev, pin1, pin2, pin3, pin4);
	...
}

loop () {
	...
	myStepper_setSpeed(60);
	myStepper_step(100);
	...
}

```


#### 5 phase support can be deactivated to save code space

5 phase motors are not very common, but supporting them increases the code
size significantly. By defining the compiler flag NO_5PHASE it is possible
to deactivate the support for 5 phase motors. This saves 736 bytes of code
space. Add this line to your Makefile if you don't need 5 phase support:

```make
CFLAGS = -DNO_5PHASE
```



## Example

Notice the slightly different position of the opening parenthesis at the
"class constructor" 'function' Stepper compared to the C++ instantiation.
(available in the examples folder as `stepper_oneRevolution.c`)


C-Version for use with sduino:

```c
#include <Arduino.h>
#include <Stepper.h>

// change this to fit the number of steps per revolution for your motor
#define stepsPerRevolution 200

// initialize the stepper library on pins 8 through 11:
Stepper (myStepper,stepsPerRevolution, 8, 9, 10, 11);

void setup() {
  // set the speed at 60 rpm:
  myStepper_setSpeed(60);
}

void loop() {
  // step one revolution  in one direction:
  myStepper_step(stepsPerRevolution);
  delay(500);

  // step one revolution in the other direction:
  myStepper_step(-stepsPerRevolution);
  delay(500);
}
```

Original Arduino C++-Sytax:

```c++
#include <Stepper.h>

const int stepsPerRevolution = 200;  // change this to fit the number of steps per revolution
// for your motor

// initialize the stepper library on pins 8 through 11:
Stepper myStepper(stepsPerRevolution, 8, 9, 10, 11);

void setup() {
  // set the speed at 60 rpm:
  myStepper.setSpeed(60);
}

void loop() {
  // step one revolution  in one direction:
  myStepper.step(stepsPerRevolution);
  delay(500);

  // step one revolution in the other direction:
  myStepper.step(-stepsPerRevolution);
  delay(500);
}
```




## Implementation details

Technically, each Stepper instance is represented by a file-descriptor-like
structure. This structure is initialized by the instantiation macro
`Stepper`. This means that all values are required to be constant values,
known at compile time. A `const` qualifier is not enough for SDCC, it
requires either a `#define` or an `enum`:

```c
const int numOfSteps = 1;	// does not work with SDCC
#define MotorPin1  10		// ok, works
enum { MotorPin2 = 11 };	// ok, works

Stepper (myStepper, numOfSteps, MotorPin1, MotorPin2);
```


All functions require a pointer to the stepper data structur as their first
argument to identify the stepper instance. All this pointer handling is
hidden by preprocessor magic, but it is still real and pure C. If you
insist, you can still call the 'real' functions yourself (but you gain
nothing, the compiler produces exactly the same output).




## Possible impovements

This is not a to-do-list, just brainstorming and a collection of random
thoughts.

Nothing yet.
