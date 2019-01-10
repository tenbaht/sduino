#include "HardwareSerial.c.h"
/**
 */
void HardwareSerial_begin_config(unsigned long baud, uint8_t config)
{
#ifdef USE_SPL
  UARTx_StopBits_TypeDef	stopbits;
  UARTx_Parity_TypeDef		parity;
  UARTx_WordLength_TypeDef	wordlength;

  wordlength = (config & 0x10) ? UARTx_WORDLENGTH_9D : UARTx_WORDLENGTH_8D;
  stopbits   = (config & 0x20) ? UARTx_STOPBITS_2   : UARTx_STOPBITS_1;
  parity=UARTx_PARITY_NO;	// default
  config &= 0x6;
  if      (config == 0x4) parity=UARTx_PARITY_EVEN;
  else if (config == 0x6) parity=UARTx_PARITY_ODD;
  
  //set the data bits, parity, and stop bits
  UARTx_Init(baud, wordlength, stopbits, parity, 
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

	UARTx->CR1 = config & (MASK_DATABITS | MASK_PARITY);
	UARTx->CR3 = config & (MASK_STOPBITS);
	// enable RXNE interrupt, enable transmitter, enable receiver
	UARTx->CR2 = UARTx_CR2_RIEN | UARTx_CR2_TEN | UARTx_CR2_REN;
#endif
  initialized = 1;
  runSerialEvent = 0;
}

