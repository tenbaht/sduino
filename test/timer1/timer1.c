/*
 * test timer4, show the value for millis() over time
 */

#include "Arduino.h"
#include "HardwareSerial.h"
#include "Print.h"


void setup(void)
{
    HardwareSerial_begin(115200);
}


void loop (void)
{
	uint32_t i;

	Print_print_s("millis()=");
	Print_print_u(millis());
	Print_print_s("\tTIM4_CNTR=");
	Print_println_u(TIM4->CNTR);

	for (i=40000; i; i--);
}
