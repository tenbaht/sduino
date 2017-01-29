
/*
 Stepper Motor Control - one revolution

 This program drives a unipolar or bipolar stepper motor.
 The motor is attached to digital pins 8 - 11 of the Arduino.

 The motor should revolve one revolution in one direction, then
 one revolution in the other direction.


 Created 11 Mar. 2007
 Modified 30 Nov. 2009
 by Tom Igoe
 Modified 28 Jan. 2017 to C syntax for use with sduino environment
 by Michael Mayer

 */

#include <Arduino.h>
#include <Serial.h>
#include <Stepper.h>

const int stepsPerRevolution = 200;  // change this to fit the number of steps per revolution
// for your motor

Stepper myStepper;

void setup() {
  // initialize the stepper library on pins 8 through 11:
  myStepper = Stepper_4phase(stepsPerRevolution, 8, 9, 10, 11);
  // set the speed at 60 rpm:
  Stepper_setSpeed(myStepper, 60);
  // initialize the serial port:
  Serial_begin(9600);
}

void loop() {
  // step one revolution  in one direction:
  Serial_println_s("clockwise");
  Stepper_step(myStepper, stepsPerRevolution);
  delay(500);

  // step one revolution in the other direction:
  Serial_println_s("counterclockwise");
  Stepper_step(myStepper, -stepsPerRevolution);
  delay(500);
}

