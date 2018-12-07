// Wire Master Reader
// by Nicholas Zambetti <http://www.zambetti.com>

// Demonstrates use of the Wire library
// Reads data from an I2C/TWI slave device
// Refer to the "Wire Slave Sender" example for use with this

// Adapted to plain C for SDunio by M. Mayer 29 November 2018
// Created 29 March 2006

// This example code is in the public domain.


#include <Wire.h>

void setup() {
  Wire_begin();        // join i2c bus (address optional for master)
  Serial_begin(9600);  // start serial for output
}

void loop() {
  Wire_requestFrom(8, 6);	// request 6 bytes from slave device #8

  while (Wire_available()) {	// slave may send less than requested
    char c = Wire_read();	// receive a byte as character
    Serial_print_c(c);		// print the character
  }

  delay(500);
}
