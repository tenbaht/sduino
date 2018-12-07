// I2C Digital Potentiometer
// by Nicholas Zambetti <http://www.zambetti.com>
// and Shawn Bonkowski <http://people.interaction-ivrea.it/s.bonkowski/>

// Demonstrates use of the Wire library
// Controls AD5171 digital potentiometer via I2C/TWI

// Adapted to plain C for SDunio by M. Mayer 27 November 2018
// Created 31 March 2006

// This example code is in the public domain.


#include <Wire.h>

void setup() {
  Wire_begin(); // join i2c bus (address optional for master)
}

byte val = 0;

void loop() {
  Wire_beginTransmission(44); // transmit to device #44 (0x2c)
  // device address is specified in datasheet
  Wire_write(0x00);            // sends instruction byte
  Wire_write(val);             // sends potentiometer value byte
  Wire_endTransmission();     // stop transmitting

  val++;        // increment value
  if (val == 64) { // if reached 64th position (max)
    val = 0;    // start over from lowest value
  }
  delay(500);
}
