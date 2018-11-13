/*
 * Simple demonstration for the PCF8574 I/O extender
 *
 * This can be used with the wide spread PCF8574 LCD display backpack boards.
 * Bits 0..2: input (LCD display uses them for RS, RW, EN)
 * Bits 3..7: output.
 *
 * On the backpack modules Bit 3 is connected to a transistor and meant to
 * drive the backlight LED of the connected LCD. The transistor is connected
 * without a resistor. This means that this bit will always read out as 0,
 * because the basis-emitter diode pulls this pin down to GND.
 *
 * This is in the public domain
 * (c) 2018 M. Mayer
 */

#include "I2C.h"

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
	Serial_begin(9600);
	Serial_println_s("Write to PCF8574");

	I2C_begin();
//	I2C_setSpeed(1);	// use fast mode
	I2C_timeOut(50);
}


void loop()
{
	static uint16_t last_time=0;	// 16 bit is enough for up to 65.6 sec
	static uint8_t i=0;
	uint8_t val;

	if ((((uint16_t)millis())-last_time) >= INTERVAL)
	{
		last_time += INTERVAL;
//		I2C_write(0x27, (i<<3) | 7);	// write 1 into P0..P2
		I2C_read_reg(0x27, (i<<3) | 7, 3);// write 1 into P0..P2, read 1 byte
		Serial_print_s("loop ");
		Serial_print_u(i);
		Serial_print_s(": 0x");

//		I2C_read(0x27, 1);		// read 1 byte into buffer
		val = I2C_receive();
		Serial_print_ub(val, 16);
		Serial_print_s(", bin=");
		printbin(val);
		Serial_println_s(NULL);
		i = (i+1) & 31;
	}
}
