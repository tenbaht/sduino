#include "HardwareSerial.c.h"
/**
 */
int HardwareSerial_available(void)
{
  return (unsigned int)(SERIAL_BUFFER_SIZE + rx_buffer.head - rx_buffer.tail) % SERIAL_BUFFER_SIZE;
}

