#include "stm8l.h"

int main() {
	int d;
	// Configure pins
	PB_DDR = 0x20;
	PB_CR1 = 0x20;
	// Loop
	do {
		PB_ODR ^= 0x20;
		for(d = 0; d < 29000; d++) { }
	} while(1);
}
