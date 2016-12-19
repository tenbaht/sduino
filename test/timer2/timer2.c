/*
 * test timer4: show the value for millis() over time and use the 
 * delay() function
 */

#include "Arduino.h"
#include "HardwareSerial.h"
#include "Print.h"


uint32_t prev=0;


void mydelay(unsigned long ms)
{
	uint32_t start = micros();

	while (ms > 0) {
//		yield();
		while ( ms > 0 && (micros() - start) >= 1000) {
			ms--;
			start += 1000;
		}
	}
}


void setup(void)
{
    HardwareSerial_begin(115200);
}


void loop (void)
{
	uint32_t now;

	now = millis();
	Print_print_s("millis()=");
	Print_print_u(now);
	Print_print_s("\tdelta=");
	Print_print_u(now-prev);
	Print_print_s("\tmicros()=");
	Print_print_u(micros());
	Print_print_s("\tTIM4_CNTR=");
	Print_println_u(TIM4->CNTR);

	prev = now;
	mydelay(1000);
}
