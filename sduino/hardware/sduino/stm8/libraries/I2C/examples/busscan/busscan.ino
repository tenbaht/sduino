/*
 * Scan the I2C bus for connected devices.
 *
 * The output is on the serial output.
 *
 * This is in the public domain
 * (c) 2018 M. Mayer
 */

#include "I2C.h"

void setup()
{
	Serial_begin(9600);
	Serial_println_s("Scanning the I2C bus for connected devices:");

	I2C_begin();
	I2C_scan();

	Serial_println_s("Scan finished.       ");
}

void loop()
{
	// Do nothing here...
}
