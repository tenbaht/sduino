/*
 * test timer4, show the value for millis() over time
 */

#include "Arduino.h"
#include "Serial.h"


void setup(void)
{
    Serial_begin(115200);
}


void loop (void)
{
	uint32_t i;

	Serial_print_s("millis()=");
	Serial_print_u(millis());
	Serial_print_s("\tTIM4_CNTR=");
	Serial_println_u(TIM4->CNTR);

	for (i=40000; i; i--);
}
