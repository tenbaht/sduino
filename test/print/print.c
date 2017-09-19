#include "Arduino.h"
#include "Serial.h"

void setup (void)
{
	Serial_begin(115200);
	Serial_println_s("\nPrint test");

	Serial_println_ib(1234,10);
	Serial_println_ib(1234,16);
	Serial_println_ib(1234,2);

	Serial_println_u(2345);
	Serial_println_u(-3);
	Serial_println_i(-3);

	Serial_println_fd(PI,10);
	Serial_println_fd(PI,11);
	Serial_println_fd(PI,12);
	Serial_println_fd(-PI,13);
}

void loop()
{
	char c;

	if (Serial_available()) {
		c = Serial_read();
		Serial_print_s("character read: 0x");
		Serial_println_ub(c,HEX);
	}
}
