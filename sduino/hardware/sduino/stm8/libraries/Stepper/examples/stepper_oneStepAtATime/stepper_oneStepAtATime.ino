
/*
 Stepper Motor Control - one step at a time

 This program drives a unipolar or bipolar stepper motor.
 The motor is attached to digital pins 8 - 11 of the Arduino.

 The motor will step one step at a time, very slowly.  You can use this to
 test that you've got the four wires of your stepper wired to the correct
 pins. If wired correctly, all steps should be in the same direction.

 Use this also to count the number of steps per revolution of your motor,
 if you don't know it.  Then plug that number into the oneRevolution
 example to see if you got it right.

 Created 30 Nov. 2009
 by Tom Igoe
 Modified 28 Jan. 2017 to C syntax for use with sduino environment
 by Michael Mayer

 */

#include <Serial.h>
#include <Stepper.h>

#define stepsPerRevolution 200  // change this to fit the number of steps per revolution
// for your motor

// initialize the stepper library on pins 8 through 11:
Stepper (myStepper,stepsPerRevolution, 8, 9, 10, 11);

int stepCount = 0;         // number of steps the motor has taken

void setup() {
  // initialize the serial port:
  Serial_begin(9600);
}

void loop() {
  // step one step:
  myStepper_step(1);
  Serial_print_s("steps:");
  Serial_println_u(stepCount);
  stepCount++;
  delay(500);
}

