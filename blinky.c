#include "stm8l.h"

int main() {
	int d;
	// Configure pins
	PE_DDR = 0x80;
	PE_CR1 = 0x80;
	// Loop
	do {
		PE_ODR ^= 0x80;
		for(d = 0; d < 29000; d++) { }
	} while(1);
}
