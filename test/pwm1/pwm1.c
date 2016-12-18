/*
 * test timer4, show the value for millis() over time
 */

#include "Arduino.h"
#include "HardwareSerial.h"
#include "Print.h"

#define INC	1

uint8_t val;
int8_t	inc;

void setup(void)
{
	int8_t i;

	HardwareSerial_begin(115200);

//	pinMode( 7, OUTPUT);
//	pinMode(12, OUTPUT);
//	pinMode( 2, OUTPUT);
	i=16; while(i) pinMode(--i, OUTPUT);

	val = 0;
	inc = INC;
}


void loop (void)
{
	int8_t i;

//	analogWrite( 7,val);
//	analogWrite(12,val);
//	analogWrite( 2,val);
	i=16; while(i) analogWrite(--i, val);

	printStr("val=");
	Print_println_u(val);

	delay(100);

	if ( ((inc>0) & (val>(255-INC)))
	   ||((inc<0) & (val<INC))   ) inc = -inc;
	val += inc;
}
