/*
 * test timer4: show the value for millis() over time and compare
 * different impementations for the delay() function
 */

#include "Arduino.h"
#include "Serial.h"


uint32_t prev=0;

void mydelay(unsigned long ms)
#if 1
{
	// 88 bytes
	uint16_t start;

	start = (uint16_t) micros();

	while (ms > 0) {
		while ( (ms > 0) && (((uint16_t)micros() - start) >= 1000)) {
			ms--;
			start += 1000;
		}

	}
}
#endif
#if 0
{
	// 92 bytes
	uint16_t start, now;

	start = (uint16_t) micros();

	while (ms > 0) {

		now = (uint16_t) micros();
		while ( (ms > 0) && ((now - start) >= 1000)) {
			ms--;
			start += 1000;
		}

	}
}
#endif
#if 0
{
	// 100 bytes
	uint16_t start, passed;

	start = (uint16_t) micros();

	while (ms > 0) {

		passed = (uint16_t) micros() - start;
		while ( (ms > 0) && (passed >= 1000)) {
			ms--;
			passed -= 1000;
			start += 1000;
		}

	}
}
#endif
#if 0
{
	// 159 bytes
	uint32_t start, passed;

	start = micros();
	while (ms > 0) {
		passed = micros() - start;
		while ( (ms > 0) && (passed >= 1000)) {
			ms--;
			passed -= 1000;
			start += 1000;
		}
	}
}
#endif
#if 0
{
	// 131 bytes
	uint32_t start = micros();

	while (ms > 0) {
		while ( (ms > 0) && ((micros() - start) >= 1000)) {
			ms--;
			start += 1000;
		}
	}
}
#endif
#if 0
{
	// 131 bytes
	uint32_t start = micros();

	while (ms > 0) {
		while ( ms > 0 && (micros() - start) >= 1000) {
			ms--;
			start += 1000;
		}
	}
}
#endif

void setup(void)
{
	Serial_begin(115200);

	Serial_print_s("testing delay functions.\n"
		"Code size of current impementation: ");
	Serial_println_u((uint16_t)setup - (uint16_t)mydelay);
}


void loop (void)
{
	uint32_t now;

	now = millis();
	Serial_print_s("millis()=");
	Serial_print_u(now);
	Serial_print_s("\tdelta=");
	Serial_print_u(now-prev);
	Serial_print_s("\tmicros()=");
	Serial_print_u(micros());
	Serial_print_s("\tTIM4_CNTR=");
	Serial_println_u(TIM4->CNTR);

	prev = now;
	mydelay(1000);
}
