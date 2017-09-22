/*
 * test serialEvent()
 *
 * Read data from the serial interface an echo it back using the serialEvent()
 * function.
 *
 * written Sep. 2017 by Michael Mayer for the SDuino project
 * Code is in the public domain.
 */

#include "Arduino.h"


void setup(void)
{
    Serial_begin(9600);
    Serial_println_s("Hello World!");
}


void loop (void)
{
}

void serialEvent(void)
{
	while(HardwareSerial_available()) {
		Serial_write(Serial_read());
	};
}
