#include <string.h>
#include "stm8s.h"

int uart_write(const char *str) {
	char i;
	for(i = 0; i < strlen(str); i++) {
		while(!(USART1_SR & USART_SR_TXE));
		USART1_DR = str[i];
	}
	return(i); // Bytes sent
}


int main() {
	unsigned long i = 0;

	CLK_CKDIVR = 0x00; // Set the frequency to 16 MHz
	CLK_PCKENR1 = 0xFF; // Enable peripherals

	PD_DDR = 0x20; // Put TX line on
	PD_CR1 = 0x20;

	USART1_CR2 = USART_CR2_TEN; // Allow TX & RX
	USART1_CR3 &= ~(USART_CR3_STOP1 | USART_CR3_STOP2); // 1 stop bit
	USART1_BRR2 = 0x03; USART1_BRR1 = 0x68; // 9600 baud

	do {
		uart_write("Hello World!\r\n");
		for(i = 0; i < 147456; i++) { } // Sleep
	} while(1);
}
