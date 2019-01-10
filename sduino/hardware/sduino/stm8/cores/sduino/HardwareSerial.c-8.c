#include "HardwareSerial.c.h"
/**
 */
int HardwareSerial_read(void)
{
  // if the head isn't ahead of the tail, we don't have any characters
  if (rx_buffer.head == rx_buffer.tail) {
//  if (!runSerialEvent) {
    return -1;
  } else {
    unsigned char c = rx_buffer.buffer[rx_buffer.tail];
    rx_buffer.tail = (unsigned int)(rx_buffer.tail + 1) % SERIAL_BUFFER_SIZE;
    runSerialEvent = (rx_buffer.head != rx_buffer.tail);
    return c;
  }
}

