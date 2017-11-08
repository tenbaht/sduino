
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

#include <Serial.h>
#include <Stepper.h>

// change this to fit the number of steps per revolution for your motor
#define stepsPerRevolution 200

// initialize the stepper library on pins 8 through 11:
Stepper (myStepper,stepsPerRevolution, 8, 9, 10, 11);

void setup() {
  // set the speed at 60 rpm:
  myStepper_setSpeed(60);
  // initialize the serial port:
  Serial_begin(9600);
}

void loop() {
  // step one revolution  in one direction:
  Serial_println_s("clockwise");
  myStepper_step(stepsPerRevolution);
  delay(500);

  // step one revolution in the other direction:
  Serial_println_s("counterclockwise");
  myStepper_step(-stepsPerRevolution);
  delay(500);
}

