/*
  HardwareSerial.c - Hardware serial library for sduino
  Copyright (c) 2016 Michael Mayer

  Plain C version of HardwareSerial.cpp of the Arduino project.
  Copyright (c) 2006 Nicholas Zambetti.  All right reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
  
  Modified 23 November 2006 by David A. Mellis
  Modified 28 September 2010 by Mark Sproul
  Modified 14 August 2012 by Alarus
  Modified 15 December 2016 by Michael Mayer
*/


#include"stm8s.h"

#include "Arduino.h"
#include "HardwareSerial.h"


// device dependent definitions ////////////////////////////////////////////

#if defined(UART1)
#warning "using uart1 for HardwareSerial"
# define UARTx			UART1
# define UARTx_RX_IRQHandler	UART1_RX_IRQHandler
# define ITC_IRQ_UARTx_RX	ITC_IRQ_UART1_RX
# define UARTx_TX_IRQHandler	UART1_TX_IRQHandler
# define ITC_IRQ_UARTx_TX	ITC_IRQ_UART1_TX
# define UARTx_CR2_TIEN		UART1_CR2_TIEN
# define UARTx_CR2_RIEN		UART1_CR2_RIEN
# define UARTx_CR2_TEN		UART1_CR2_TEN
# define UARTx_CR2_REN		UART1_CR2_REN
# define UARTx_FLAG_PE		UART1_FLAG_PE
# define UARTx_FLAG_TC		UART1_FLAG_TC
# define UARTx_DeInit		UART1_DeInit
# ifdef USE_SPL
#  define UARTx_ReceiveData8	UART1_ReceiveData8
#  define UARTx_GetFlagStatus	UART1_GetFlagStatus
#  define UARTx_ITConfig	UART1_ITConfig
#  define UARTx_Init		UART1_Init
#  define UARTx_IT_TXE		UART1_IT_TXE
#  define UARTx_IT_RXNE		UART1_IT_RXNE
#  define UARTx_WORDLENGTH_8D	UART1_WORDLENGTH_8D
#  define UARTx_WORDLENGTH_9D	UART1_WORDLENGTH_9D
#  define UARTx_STOPBITS_1	UART1_STOPBITS_1
#  define UARTx_STOPBITS_2	UART1_STOPBITS_2
#  define UARTx_SYNCMODE_CLOCK_DISABLE	UART1_SYNCMODE_CLOCK_DISABLE
#  define UARTx_MODE_TXRX_ENABLE	UART1_MODE_TXRX_ENABLE
#  define UARTx_PARITY_NO	UART1_PARITY_NO
#  define UARTx_PARITY_EVEN	UART1_PARITY_EVEN
#  define UARTx_PARITY_ODD	UART1_PARITY_ODD
#  define UARTx_StopBits_TypeDef	UART1_StopBits_TypeDef
#  define UARTx_Parity_TypeDef	UART1_Parity_TypeDef
#  define UARTx_WordLength_TypeDef	UART1_WordLength_TypeDef
# endif
#elif defined(UART2)
#warning "using uart2 for HardwareSerial"
# define UARTx			UART2
# define UARTx_RX_IRQHandler	UART2_RX_IRQHandler
# define ITC_IRQ_UARTx_RX	ITC_IRQ_UART2_RX
# define UARTx_TX_IRQHandler	UART2_TX_IRQHandler
# define ITC_IRQ_UARTx_TX	ITC_IRQ_UART2_TX
# define UARTx_CR2_TIEN		UART2_CR2_TIEN
# define UARTx_CR2_RIEN		UART2_CR2_RIEN
# define UARTx_CR2_TEN		UART2_CR2_TEN
# define UARTx_CR2_REN		UART2_CR2_REN
# define UARTx_FLAG_PE		UART2_FLAG_PE
# define UARTx_FLAG_TC		UART2_FLAG_TC
# define UARTx_DeInit		UART2_DeInit
# ifdef USE_SPL
#  define UARTx_ReceiveData8	UART2_ReceiveData8
#  define UARTx_GetFlagStatus	UART2_GetFlagStatus
#  define UARTx_ITConfig	UART2_ITConfig
#  define UARTx_Init		UART2_Init
#  define UARTx_IT_TXE		UART2_IT_TXE
#  define UARTx_IT_RXNE		UART2_IT_RXNE
#  define UARTx_WORDLENGTH_8D	UART2_WORDLENGTH_8D
#  define UARTx_WORDLENGTH_9D	UART2_WORDLENGTH_9D
#  define UARTx_STOPBITS_1	UART2_STOPBITS_1
#  define UARTx_STOPBITS_2	UART2_STOPBITS_2
#  define UARTx_SYNCMODE_CLOCK_DISABLE	UART2_SYNCMODE_CLOCK_DISABLE
#  define UARTx_MODE_TXRX_ENABLE	UART2_MODE_TXRX_ENABLE
#  define UARTx_PARITY_NO	UART2_PARITY_NO
#  define UARTx_PARITY_EVEN	UART2_PARITY_EVEN
#  define UARTx_PARITY_ODD	UART2_PARITY_ODD
#  define UARTx_StopBits_TypeDef	UART2_StopBits_TypeDef
#  define UARTx_Parity_TypeDef	UART2_Parity_TypeDef
#  define UARTx_WordLength_TypeDef	UART2_WordLength_TypeDef
# endif
#else
# error "no UART definition found."
#endif


#ifdef NO_SERIAL
/*
 * empty default IRQ functions to make the linker happy if the
 * respective module is not to linked.
 */

void UARTx_TX_IRQHandler(void) __interrupt(ITC_IRQ_UARTx_TX){}
void UARTx_RX_IRQHandler(void) __interrupt(ITC_IRQ_UARTx_RX){}
#else // ifdef NO_SERIAL


// private data //////////////////////////////////////////////////////////////

#define SERIAL_BUFFER_SIZE 16
typedef struct ring_buffer
{
  unsigned char buffer[SERIAL_BUFFER_SIZE];
  volatile unsigned int head;
  volatile unsigned int tail;
} ring_buffer;

static ring_buffer rx_buffer;// = { { 0 }, 0, 0};
static ring_buffer tx_buffer;// = { { 0 }, 0, 0};

static volatile char	transmitting;//=0;
static unsigned char initialized;//=0 internal status. Returned on HardwareSerial()

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

// Public Methods //////////////////////////////////////////////////////////////

uint8_t HardwareSerial(void)
{
    return initialized;
}


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

int HardwareSerial_available(void)
{
  return (unsigned int)(SERIAL_BUFFER_SIZE + rx_buffer.head - rx_buffer.tail) % SERIAL_BUFFER_SIZE;
}

int HardwareSerial_peek(void)
{
  if (rx_buffer.head == rx_buffer.tail) {
//  if (!runSerialEvent) {
    return -1;
  } else {
    return rx_buffer.buffer[rx_buffer.tail];
  }
}

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

#endif // ifdef NO_SERIAL
