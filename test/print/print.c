#include "Arduino.h"
#include "Print.h"

void setup (void)
{
	printNumber(1234,10);	printChr(10);
	printNumber(1234,16);	printChr(10);
	printNumber(1234,2);	printChr(10);

	Print_println_u(2345);
	Print_println_u(-3);
	Print_println_i(-3);

	Print_printFloat(PI,10);	printChr(10);
	Print_printFloat(PI,11);	printChr(10);
	Print_printFloat(PI,12);	printChr(10);
	Print_printFloat(-PI,13);	printChr(10);
}

void loop(){}
