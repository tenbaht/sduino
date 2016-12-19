/*
 * test adc1, read all five channels
 */

#include "Arduino.h"
#include "HardwareSerial.h"
#include "Print.h"

#include "stm8s_flash.h"

void setup(void)
{
	HardwareSerial_begin(115200);

	// configure analog pins for input
	pinMode(A0, INPUT);
	pinMode(A1, INPUT);
	pinMode(A2, INPUT);
	pinMode(A3, INPUT);
	pinMode(A4, INPUT);
}


void loop (void)
{
	uint8_t i;

	for (i=0; i<5; ++i) {
		// use channel numbers for reading the inputs
		Print_print_u(analogRead(i));
		printStr("\t");
	};
	println();

	delay(200);
}
