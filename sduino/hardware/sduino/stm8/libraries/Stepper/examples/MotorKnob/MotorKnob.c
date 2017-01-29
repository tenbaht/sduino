/*
 * MotorKnob
 *
 * A stepper motor follows the turns of a potentiometer
 * (or other sensor) on analog input 0.
 *
 * http://www.arduino.cc/en/Reference/Stepper
 * This example code is in the public domain.
 */

#include <Arduino.h>
#include <Stepper.h>

// change this to the number of steps on your motor
#define STEPS 200

Stepper myStepper;

// the previous reading from the analog input
int previous = 0;

void setup() {
  // create an instance of the stepper class, specifying
  // the number of steps of the motor and the pins it's
  // attached to
  myStepper = Stepper_4phase(STEPS, 8, 9, 10, 11);
  // set the speed of the motor to 30 RPMs
  Stepper_setSpeed(myStepper,30);
}

void loop() {
  // get the sensor value
  int val = analogRead(0);

  // move a number of steps equal to the change in the
  // sensor reading
  Stepper_step(myStepper, val - previous);

  // remember the previous value of the sensor
  previous = val;
}
