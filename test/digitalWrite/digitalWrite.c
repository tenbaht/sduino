/*
 * test functions for digitalWrite using the sduino environment
 */

#include "Arduino.h"

/* A reference of main() is required to properly pull in main() from the
 * core library. For .ino and .pde files this is done automatically by the
 * sduino.mk makefile include, for .c files this has to be done by the user.
 */
//void main(void);
//void (*dummy_variable) () = main;

void setup (void)
{
	runSerialEvent = 0;	// alternative way to pull in main.c
	pinMode(1, OUTPUT);
}

void loop (void)
{
	digitalWrite(1,HIGH);
	digitalWrite(1,LOW);
	digitalWrite(analogInputToDigitalPin(1),LOW);
}
