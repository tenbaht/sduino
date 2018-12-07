/*
 * Simple demonstration for the PCF8574 I/O extender
 *
 * Blink an LED, show a counting pattern on some pins and read back three
 * input lines.
 *
 * This can be used with the wide spread PCF8574 LCD display backpack boards.
 * Bits 0..2: input (LCD display uses them for RS, RW, EN)
 * Bits 3..7: output.
 *
 * Connections (with LCD):
 * connect the LCD. Only the backlight will be used, there won't be any text
 * on the LCD.
 * Bit 0, 1: (optinal) connect via a switch/button to GND
 *
 * Connections (without LCD):
 * Use an LED or a simple logic analyser to see the counting pattern.
 * Bit 0..2: connect via a switch/button to GND
 * Bit 3: To anode of LED, cathode via 1k resistor to Vcc (5V)
 * Bit 4..7: (optional) to logic analyser or oszilloscope to see the
 *           counting pattern
 * 
 * On the backpack modules Bit 3 is connected to a transistor and meant to
 * drive the backlight LED of the connected LCD. The transistor is connected
 * without a resistor. This means that this bit will always read out as 0,
 * because the basis-emitter diode pulls this pin down to GND.
 *
 * This is in the public domain
 * (c) 2018 M. Mayer
 */

#include "Wire.h"

// time interval for the counting (in ms per step)
#define INTERVAL 50

/**
 * print a value in binary.
 *
 * Needed because the standard function Print_ub(val,base) does not print
 * leading zeros.
 */
void printbin(uint8_t v)
{
	uint8_t i;

	for (i=8; i--;)
	{
		Serial_print_c('0'+(v>=128));
		v <<= 1;
	}
}


void setup()
{
	Serial_begin(115200);
	Serial_println_s("Write to PCF8574");

	Wire_begin();
}


void loop()
{
	static uint16_t last_time=0;	// 16 bit is enough for up to 65.6 sec
	static uint8_t i=0;
	uint8_t val;

	if ((((uint16_t)millis())-last_time) >= INTERVAL)
	{
		last_time += INTERVAL;

		Serial_print_s("loop ");
		Serial_print_u(i);
		Serial_print_c(':');

/*
 * For demonstration purposes this sketch uses two different ways for
 * communication. There is no technical need for this split.
 * - loop 0..15:
 *   Sending with begin/endTransmission, receiving with requestFrom
 * - loop 16..31:
 *   Sending and receiving in one combined call of requestFrom
 */
		if (i<16)
		{
			// a) show how to use begin/endTransmission():

			// send data
			Wire_beginTransmission(0x27);
			Wire_print_s("abc");		// just to see some traffic
			Wire_write((i<<3) | 7);		// write 1 into P0..P2
			Wire_endTransmission();

			// receive data
			Wire_requestFrom(0x27, 2);	// request 2 bytes from slave device 0x27
		}
		else
		{
			// b) show how to use the combined transfer:

			// Send one data byte and request 2 bytes from slave
			// device 0x27
			Wire_requestFrom(0x27, 2, (i<<3)|7, 1, true);
		}

		while (Wire_available())	// slave may send less than requested
		{
			val = Wire_read();	// receive a byte
			Serial_print_s(" 0x");
			Serial_print_ub(val,16);// print the value
			Serial_print_s("/0b");
			printbin(val);
		}

		Serial_println();
		i = (i+1) & 31;
	}
}
