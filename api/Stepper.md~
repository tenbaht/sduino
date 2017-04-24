# Stepper Library

A Library for driving stepper motors with 2, 4 or 5 phases.
Derived from the Arduino Stepper library v1.8.0.


## API

This library is a multi-instance library, it supports an arbitrary number of
stepper motors per sketch.

The API syntax is very similar to the original C++ syntax, thanks to some
[c preprocessor macro magic](macro.html).

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


There is no explicit constructor method. The output mode is set
automatically right before the very first motor step.

This means that the driver pins stay uninitialized (in input mode) and are
left floating until the first step is requested. This won't be a problem in
most cases as most motor drivers are using pull-up or pull-down resistors to
ensure definite signal levels.

If needed, the application could call the (internal) `stepMotor(0)` method
in the setup() function to force an immediate port pin initialization:

```c
setup() {
  myStepper_stepMotor(0);    // sets the motor pins to output mode
}
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

If you really need to (re-) configure the pin connections at run time you could
initialize a data structure with `0` for the unknown pin numbers and set them
later by directy accessing the structure before the first use of that
instance:

```c
Stepper (myStepper,0,0);

setup() {
	... do some calculations for the pin numbers ...
	myStepper.motor_pin_1 = pin[0];
	myStepper.motor_pin_2 = pin[1];
	myStepper.motor_pin_3 = pin[2];
	myStepper.motor_pin_4 = pin[3];
	...
}

loop () {
	...
	myStepper_setSpeed(60);
	myStepper_step(100);
	...
}

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



[Back to the main page](../index.html)
