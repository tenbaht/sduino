/*
 * test timer4, show the value for millis() over time
 */

#include "Arduino.h"
#include "HardwareSerial.h"
#include "Print.h"

#include "stm8s_flash.h"

#define INC	5

uint8_t val;
int8_t	inc;

void setup(void)
{
	int8_t i;
	uint16_t opt;

	HardwareSerial_begin(115200);

//	pinMode( 7, OUTPUT);
//	pinMode(12, OUTPUT);
//	pinMode( 2, OUTPUT);
//	i=16; while(i) pinMode(--i, OUTPUT);

	printStr("opt2=0x");
	Print_print_ub(OPT->OPT2,HEX);

	printStr(" read opt2:0x");
	opt = FLASH_ReadOptionByte(OPT->OPT2);
	Print_print_ub(opt,HEX);
	printStr(" read 0x4803=");
//	FLASH_Unlock(FLASH_MEMTYPE_DATA);
	opt = FLASH_ReadOptionByte(0x4803);//OPT->OPT2);
//	FLASH_Lock(FLASH_MEMTYPE_DATA);
	Print_print_ub(opt,HEX);

	alternateFunction(1);

	printStr(" read 0x4803=");
//	FLASH_Unlock(FLASH_MEMTYPE_DATA);
	opt = FLASH_ReadOptionByte(0x4803);//OPT->OPT2);
//	FLASH_Lock(FLASH_MEMTYPE_DATA);
	Print_print_ub(opt,HEX);
	
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
