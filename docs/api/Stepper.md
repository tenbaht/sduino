# Stepper Library

For driving stepper motors with 2, 4 or 5 phases.
Derived from the Arduino Stepper library v1.8.0.

This library has a slightly diffent user interface than the usual singleton
libraries. This allows for handling more than one stepper per Sketch but it
requires some more attention when porting an existing sketch from C++ to C.

Each Stepper instance is represented by a file-descriptor-like structure.
This structure is initialized by a constructor-like function returning a
pointer to this stucture. This pointer needs to be given as the first
argument for ever other function call to identify the stepper instance.

## API table

Arduino method			| sduino function
--------------			| ---------------
				| Stepper myStepper; // global variable
Stepper myStepper(steps, pin1,pin2);		| myStepper = Stepper_2phase(steps, pin1,pin2);
Stepper myStepper(steps, pin1,pin2,pin3,pin4);	| myStepper = Stepper_4phase(steps,pin1,pin2,pin3,pin4);
Stepper myStepper(steps, pin1,pin2,pin3,pin4,pin5);| myStepper = Stepper_5phase(steps,pin1,pin2,pin3,pin4,pin5);
myStepper.setSpeed(speed);	| Stepper_setSpeed(myStepper, speed);
myStepper.step(n);		| Stepper_step(myStepper, n);
myStepper.version();		| Stepper_version();



## Example

(available in the examples folder as `stepper_oneRevolution.c`)
Notice the Variable `myStepper` of type `Stepper` that is intialized by a
constructor-like function `Stepper_4phase`:

C-Version for use with sduino:

```c
#include <Arduino.h>
#include <Stepper.h>

const int stepsPerRevolution = 200;  // change this to fit the number of steps p
// for your motor

Stepper myStepper;

void setup() {
  // initialize the stepper library on pins 8 through 11:
  myStepper = Stepper_4phase(stepsPerRevolution, 8, 9, 10, 11);
  // set the speed at 60 rpm:
  Stepper_setSpeed(myStepper, 60);
}

void loop() {
  // step one revolution  in one direction:
  Stepper_step(myStepper, stepsPerRevolution);
  delay(500);

  // step one revolution in the other direction:
  Stepper_step(myStepper, -stepsPerRevolution);
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


## API

```
#include <Stepper.h>

// plain C interface for use with SDCC

// use a pointer to struct Stepper_s as a class instance aquivalent
typedef struct Stepper_s *Stepper;

// "constructor" function
Stepper	Stepper_5phase(
        unsigned int number_of_steps,
        unsigned char motor_pin_1,
        unsigned char motor_pin_2,
        unsigned char motor_pin_3,
        unsigned char motor_pin_4,
        unsigned char motor_pin_5);

// use #defines to emulate the polymorph class constructor in C++
#define Stepper_2phase(N,A,B)		Stepper_5phase((N),(A),(B),0,0,0)
#define Stepper_4phase(N,A,B,C,D)	Stepper_5phase((N),(A),(B),(C),(D),0)

// speed setter method:
void Stepper_setSpeed(Stepper this, long whatSpeed);

// mover method:
void Stepper_step(Stepper this, int number_of_steps);

// replace the version() method with a simple define
#define Stepper_version()	5

// (not so) private:
void Stepper_stepMotor(Stepper s, int this_step);
```

[Back to the main page](../index.html)
