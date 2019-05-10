#include "HardwareSerial.c.h"
/**
 */
size_t HardwareSerial_write(uint8_t c)
{
  int i = (tx_buffer.head + 1) % SERIAL_BUFFER_SIZE;
	
  // If the output buffer is full, there's nothing for it other than to 
  // wait for the interrupt handler to empty it a bit
  // ???: return 0 here instead?
  while (i == tx_buffer.tail)
    ;
	
  tx_buffer.buffer[tx_buffer.head] = c;
  tx_buffer.head = i;

#ifdef USE_SPL
  UARTx_ITConfig(UARTx_IT_TXE, ENABLE);		// enable TXE interrupt
#else
  UARTx->CR2 |= UARTx_CR2_TIEN;			// enable TXE interrupt
#endif
  transmitting = 1;
  //FIXME: unklar, ob das auf dem STM8 wirklich nötig ist.
  // Das TXE-Bit in UART_SR ist jedenfalls nur lesbar.
  // clear the TXC bit -- "can be cleared by writing a one to its bit location"
//  sbi(*_ucsra, TXC0);
  
  return 1;
}
