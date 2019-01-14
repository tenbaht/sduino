#include "HardwareSerial.c.h"
/**
 */
int HardwareSerial_peek(void)
{
  if (rx_buffer.head == rx_buffer.tail) {
//  if (!runSerialEvent) {
    return -1;
  } else {
    return rx_buffer.buffer[rx_buffer.tail];
  }
}

