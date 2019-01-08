#include "HardwareSerial.c.h"
/**
 */
void HardwareSerial_begin(unsigned long baud)
{
#ifdef USE_SPL
  //set the data bits, parity, and stop bits
  UARTx_Init(baud,
      UARTx_WORDLENGTH_8D, UARTx_STOPBITS_1, UARTx_PARITY_NO,
      UARTx_SYNCMODE_CLOCK_DISABLE, UARTx_MODE_TXRX_ENABLE);

  UARTx_ITConfig(UARTx_IT_RXNE, ENABLE);	// enable RXNE interrupt
#else
	uint16_t divider;

	/* Set the UARTx BaudRates in BRR1 and BRR2 registers according to UARTx_BaudRate value */
	divider = (uint16_t) ((uint32_t)CLK_GetClockFreq() / (uint32_t) baud);

	UARTx->BRR2 = divider & 0x0f;
	divider >>= 4;
	UARTx->BRR1 = divider & 0xff;
	divider >> 4;
	UARTx->BRR2 |= (uint8_t) divider & 0xf0;

	UARTx->CR1 = 0;			// 8 Bit, no parity
	UARTx->CR3 = 0;			// one stop bit
	// enable RXNE interrupt, enable transmitter, enable receiver
	UARTx->CR2 = UARTx_CR2_RIEN | UARTx_CR2_TEN | UARTx_CR2_REN;
#endif
  initialized = 1;
  runSerialEvent = 0;
}

