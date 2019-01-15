#include "HardwareSerial.c.h"
/**
 * This part contains the bare minimum of serial support functions
 *
 * The interrupt functions (and the supporting basic data structures) can't
 * be removed by the linker, even if no serial functions are used in the sketch.
 *
 * Add a "-DNO_SERIAL" to the CPPFLAGS or CFLAGS to full remove this code.
 */

#ifdef NO_SERIAL
/*
 * empty default IRQ functions to make the linker happy if the
 * respective module is not to linked.
 */

void UARTx_RX_IRQHandler(void) __interrupt(ITC_IRQ_UARTx_RX){}
void UARTx_TX_IRQHandler(void) __interrupt(ITC_IRQ_UARTx_TX){}

#else // ifdef NO_SERIAL

// private data //////////////////////////////////////////////////////////////

// These variables should be static, but that doesn't go together with
// splitting the source code into smaller units.

ring_buffer rx_buffer;// = { { 0 }, 0, 0};
ring_buffer tx_buffer;// = { { 0 }, 0, 0};

volatile char	transmitting;//=0;
unsigned char initialized;//=0 internal status. Returned on HardwareSerial()



// private functions  ////////////////////////////////////////////////////////

static void store_char(unsigned char c, ring_buffer *buffer)
//inline void store_char(unsigned char c, ring_buffer *buffer)
{
  int i = (unsigned int)(buffer->head + 1) % SERIAL_BUFFER_SIZE;

  // if we should be storing the received character into the location
  // just before the tail (meaning that the head would advance to the
  // current location of the tail), we're about to overflow the buffer
  // and so we don't write the character or advance the head.
  if (i != buffer->tail) {
    buffer->buffer[buffer->head] = c;
    buffer->head = i;
    runSerialEvent = 1;
  }
}

// Interrupt handler ///////////////////////////////////////////////////////////

void UARTx_RX_IRQHandler(void) __interrupt(ITC_IRQ_UARTx_RX) /* UART1/2 RX */
{
    unsigned char c;

#ifdef USE_SPL
    c = UARTx_ReceiveData8();
    // check for parity error
    if (!UARTx_GetFlagStatus(UARTx_FLAG_PE)) {
#else
    c = UARTx->DR;
    // check for parity error
    if (!(UARTx->SR & UARTx_FLAG_PE)) {
#endif
        // no parity error, so store the data
        store_char(c, &rx_buffer);
    };
}


void UARTx_TX_IRQHandler(void) __interrupt(ITC_IRQ_UARTx_TX) /* UART1/2 TX */
{
#ifdef USE_SPL
  if (tx_buffer.head == tx_buffer.tail) {
	// Buffer empty, so disable interrupts
        transmitting = 0;
        UARTx_ITConfig(UARTx_IT_TXE, DISABLE);
  }
  else {
    // There is more data in the output buffer. Send the next byte
    unsigned char c = tx_buffer.buffer[tx_buffer.tail];
    tx_buffer.tail = (tx_buffer.tail + 1) % SERIAL_BUFFER_SIZE;
	
    UARTx_SendData8(c);
  }
#else
  if (tx_buffer.head == tx_buffer.tail) {
	// Buffer empty, so disable interrupts
        transmitting = 0;
	UARTx->CR2 &= ~UARTx_CR2_TIEN;
  }
  else {
    // There is more data in the output buffer. Send the next byte
    unsigned char c = tx_buffer.buffer[tx_buffer.tail];
    tx_buffer.tail = (tx_buffer.tail + 1) % SERIAL_BUFFER_SIZE;

    UARTx->DR = c;
  }
#endif
}
#endif // ifdef NO_SERIAL

// Public Methods //////////////////////////////////////////////////////////////

