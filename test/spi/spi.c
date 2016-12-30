/*
 * test spi functions
 */

#include "Arduino.h"
#include "SPI.h"
#include "Serial.h"

// pin number for the status LED
#define LED	3

/*
// 9 PC7 MISO
// 8 PC6 MOSI
// 7 PC5 SCK
// 3 PB5 LED/SS
*/

//const char testdata[]="ABCDEFGH";
const char testdata[]="UUUUUUUU";	// regular bit pattern 01010101
char buf[16];

uint8_t ledstatus;

/*
 * simple test using single byte transfers
 */
void SPI_transfer_loop(uint8_t *buf, size_t n)
{
	while (--n) {
		*buf = SPI_transfer(*buf);
		buf++;
	}
}


void setup(void)
{
	pinMode(LED, OUTPUT);
	Serial_begin(115200);
	Serial_println_s("SPI test");
}


void loop (void)
{
	uint8_t i;

	Serial_println_s("loop");
	strcpy(buf, testdata);
	delay(100);

	digitalWrite(LED,1);//ledstatus);
	ledstatus = 1-ledstatus;


	SPI_begin();
	SPI_beginTransaction(SPISettings(8000000L,MSBFIRST,SPI_MODE0));
	Serial_print_c('a');
//	SPI_transfer(0xaa);
//	SPI_transfer(0x55);
	Serial_print_c('b');

	SPI_transfer_asm(buf, sizeof(testdata)-1);
	SPI_endTransaction();
	Serial_print_c('c');
	SPI_end();
	Serial_print_c('d');

	digitalWrite(LED,0);

	for (i=0; i<8; i++) {
		Serial_print_ub(buf[i],HEX);
		Serial_print_c(' ');
	}

	delay(100);
}
