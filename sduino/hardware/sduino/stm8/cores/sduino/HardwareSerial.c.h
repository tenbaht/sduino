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


// private data //////////////////////////////////////////////////////////////

#define SERIAL_BUFFER_SIZE 16
typedef struct ring_buffer
{
  unsigned char buffer[SERIAL_BUFFER_SIZE];
  volatile unsigned int head;
  volatile unsigned int tail;
} ring_buffer;


extern ring_buffer rx_buffer;// = { { 0 }, 0, 0};
extern ring_buffer tx_buffer;// = { { 0 }, 0, 0};

extern volatile char	transmitting;//=0;
extern unsigned char initialized;//=0 internal status. Returned on HardwareSerial()



