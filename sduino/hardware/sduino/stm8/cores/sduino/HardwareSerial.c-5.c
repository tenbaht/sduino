#include "HardwareSerial.c.h"
/**
 */
void HardwareSerial_end(void)
{
  // wait for transmission of outgoing data
  while (tx_buffer.head != tx_buffer.tail)
    ;

  UARTx_DeInit();
  
  // clear any received data
  rx_buffer.head = rx_buffer.tail;
  initialized = 0;
  runSerialEvent = 0;
}

