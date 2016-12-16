/*
 * test functions for digitalWrite using the sduino environment
 */

#include "Arduino.h"

void setup (void)
{
	pinMode(1, OUTPUT);
}

void loop (void)
{
	digitalWrite(1,HIGH);
	digitalWrite(1,LOW);
	digitalWrite(analogInputToDigitalPin(1),LOW);
}
