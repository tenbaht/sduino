/*
 * test timer4, show the value for millis() over time
 */

#include "Arduino.h"
#include "Serial.h"

#include "stm8s_flash.h"

#define INC	17

uint8_t val;
int8_t	inc;


void printnib(uint8_t val)
{
	if (val>9) val+='A'-'9'-1;
	Serial_write(val+'0');
}

void printhex(uint8_t val)
{
	printnib(val>>4);
	printnib(val&15);
}


void dump_hexline(uint8_t *adr)
{
	uint8_t i;

	Serial_print_ub((uint32_t) adr, HEX);
	Serial_write(' ');
	for (i=0; i<16; i++) {
//		Print_print_ub(adr[i], HEX);
		printhex(adr[i]);
		if (i==7) Serial_write(' ');
		Serial_write(' ');
	}
}


void setup(void)
{
//	int8_t i;
	uint16_t opt;

	Serial_begin(115200);

//	pinMode( 7, OUTPUT);
//	pinMode(12, OUTPUT);
//	pinMode( 2, OUTPUT);
//	i=16; while(i) pinMode(--i, OUTPUT);

	Serial_print_s("opt2=0x");
	Serial_print_ub(OPT->OPT2,HEX);

	Serial_print_s(" read opt2:0x");
	opt = FLASH_ReadOptionByte(OPT->OPT2);
	Serial_print_ub(opt,HEX);
	Serial_print_s(" read 0x4803=");
//	FLASH_Unlock(FLASH_MEMTYPE_DATA);
	opt = FLASH_ReadOptionByte(0x4803);//OPT->OPT2);
//	FLASH_Lock(FLASH_MEMTYPE_DATA);
	Serial_print_ub(opt,HEX);

#ifdef SUPPORT_ALTERNATE_MAPPINGS
	alternateFunction(1);
#endif

	Serial_print_s(" read 0x4803=");
//	FLASH_Unlock(FLASH_MEMTYPE_DATA);
	opt = FLASH_ReadOptionByte(0x4803);//OPT->OPT2);
//	FLASH_Lock(FLASH_MEMTYPE_DATA);
	Serial_print_ub(opt,HEX);
	
	val = 0;
	inc = INC;
}


void loop (void)
{
	int8_t i;
	uint8_t *tmp;

	analogWrite(PC3,val);// T1-3, 5,val);
	analogWrite(PC4,val);// T1-4, 6,val);
	analogWrite(PD3,val);// T2-2, 12,val);
//	i=16; while(i) analogWrite(--i, val);

	Serial_print_s("val=");
	Serial_println_u(val);

	delay(200);

	if ( ((inc>0) & (val>(255-INC)))
	   ||((inc<0) & (val<INC))   ) inc = -inc;
	val += inc;

//	if (val==0) 
	dump_hexline(&TIM1->CR1);
	Serial_println();
	dump_hexline(&TIM2->CR1);
/*
	for (i=0; i<16; i++) {
		tmp = TIM1->CR1;
		Serial_print_ub(*(uint8_t*)((unsigned)TIM1->CR1+i),HEX);
		if (i==8) Serial_write(' ');
		Serial_write(' ');
	}
*/
}
