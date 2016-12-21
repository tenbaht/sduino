/*
 * test HardwareSerial.c using sduino functions
 */

#include "Arduino.h"
#include "HardwareSerial.h"



void send_string(char *str)
{
    char c;

    if (!str) return;

    while ( c=*str++ ) HardwareSerial_write(c);	// assignment intented
}


void setup(void)
{
    HardwareSerial_begin(9600);
}


void loop (void)
{
	uint32_t i;

	send_string("Hello World!\r\n");

	for (i=60000; i; i--);
	while(HardwareSerial_available()) {
		HardwareSerial_write('.');
		HardwareSerial_write(HardwareSerial_read());
	};

	for (i=60000; i; i--);
}
