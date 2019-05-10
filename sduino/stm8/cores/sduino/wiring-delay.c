#include "wiring-header.h"

/**
 * Delay for the given number of milliseconds.
 *
 * Do a busy wait. Using the lower 16 bits of micros() is enough, as the
 * difference between the start time and the current time will never be much
 * higher than 1000.
 *
 * Using unsigned values is helpful here. This way value wrap-arounds between
 * start and now do not result in negative values but in the wanted absolute
 * difference. The magic of modulo-arithmethics.
 */
void delay(unsigned long ms)
{
	uint16_t start, now;

	start = (uint16_t) micros();	// use the lower 16 bits

	while (ms > 0) {
		yield();
		now = (uint16_t) micros();	// use the lower 16 bits
		while ( (ms > 0) && ((uint16_t)(now-start) >= 1000) ) {
			ms--;
			start += 1000;
		}
	}
}


