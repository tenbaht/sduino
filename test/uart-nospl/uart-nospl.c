#include <stdint.h>

/**
 * using the UART1 without any SPL functions.
 *
 * simple testbed to test the simplified initialization function.
 *
 */

#define UART1_CR2_RIEN	0x20
#define UART1_TEN	0x08
#define UART1_REN	0x04

unsigned char
	UART1_BRR1,
	UART1_BRR2,
	UART1_CR1,
	UART1_CR2,
	UART1_CR3;


unsigned long CLK_GetClockFreq(void)
{
	return 10000000UL;
}


void HardwareSerial_begin(unsigned long baud)
{
	uint16_t divider;
	uint8_t tmp;

	/* Set the UART1 BaudRates in BRR1 and BRR2 registers according to UART1_BaudRate value */
	divider = (uint16_t) ((uint32_t)CLK_GetClockFreq() / (uint32_t) baud);

	UART1_BRR2 = divider & 0x0f;
	divider >>= 4;
	UART1_BRR1 = divider & 0xff;
	divider >>= 4;
	UART1_BRR2 |= (uint8_t) divider & 0xf0;

	UART1_CR1 = 0;			// 8 Bit, no parity
	UART1_CR3 = 0;			// one stop bit
	// enable RXNE interrupt, enable transmitter, enable receiver
	UART1_CR2 = UART1_CR2_RIEN | UART1_TEN | UART1_REN;
}


void main (void)
{
	HardwareSerial_begin(9600);
}
