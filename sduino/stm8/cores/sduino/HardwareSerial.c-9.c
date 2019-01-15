#include "HardwareSerial.c.h"
/**
 */
void HardwareSerial_flush(void)
{
  // UDR is kept full while the buffer is not empty, so TXC triggers when
  // EMPTY && SENT

//  while (transmitting && ! (*_ucsra & _BV(TXC0)));
//  while (transmitting && ! (UARTx_SR & UARTx_SR_TC));
#ifdef USE_SPL
  while (transmitting && ! UARTx_GetFlagStatus(UARTx_FLAG_TC));
#else
  while (transmitting && !(UARTx->SR & UARTx_FLAG_TC));
#endif
  transmitting = 0;
}

