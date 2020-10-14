/*
 * test adc1, read all five channels
 *
 * But skip channel 4, as it is used as TX. analogRead would mess up the
 * send data while the ADC is active.
 */

#include "Arduino.h"
#include "Serial.h"


void setup(void)
{
	Serial_begin(115200);
	Serial_println_s("ADC test");

	// configure analog pins for input
	pinMode(A0, INPUT);
	pinMode(A1, INPUT);
	pinMode(A2, INPUT);
	pinMode(A3, INPUT);	// this pin is used as TX
	pinMode(A4, INPUT);	// this pin is used as RX
}


void loop (void)
{
	uint8_t i;
	uint16_t val;

	for (i=0; i<NUM_ANALOG_INPUTS; ++i) {
		// use channel numbers for reading the inputs
		// skip channel 3 (used as TX), use dummy value 1234 instead
		val = (i==3) ? 1234 : analogRead(i);
		Serial_print_u(val);
		Serial_write('\t');
	};
	Serial_println();

	delay(200);
}
