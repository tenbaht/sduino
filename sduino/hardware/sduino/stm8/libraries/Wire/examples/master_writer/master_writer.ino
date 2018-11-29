// Wire Master Writer
// by Nicholas Zambetti <http://www.zambetti.com>

// Demonstrates use of the Wire library
// Writes data to an I2C/TWI slave device
// Refer to the "Wire Slave Receiver" example for use with this

// Adapted to plain C for SDunio by M. Mayer 27 November 2018
// Created 29 March 2006

// This example code is in the public domain.


#include <Wire.h>

void setup() {
  Wire_begin(); // join i2c bus (address optional for master)
}

byte x = 0;

void loop() {
  Wire_beginTransmission(8); // transmit to device #8
  Wire_write_s("x is ");     // sends five bytes
  Wire_write(x);             // sends one byte
  Wire_endTransmission();    // stop transmitting

  x++;
  delay(500);
}
