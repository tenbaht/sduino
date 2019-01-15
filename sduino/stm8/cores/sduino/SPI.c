/*
 * Copyright (c) 2010 by Cristian Maglie <c.maglie@arduino.cc>
 * Copyright (c) 2014 by Paul Stoffregen <paul@pjrc.com> (Transaction API)
 * Copyright (c) 2014 by Matthijs Kooijman <matthijs@stdin.nl> (SPISettings AVR)
 * Copyright (c) 2014 by Andrew J. Kroll <xxxajk@gmail.com> (atomicity fixes)
 * SPI Master library for arduino.
 *
 * This file is free software; you can redistribute it and/or modify
 * it under the terms of either the GNU General Public License version 2
 * or the GNU Lesser General Public License version 2.1, both as
 * published by the Free Software Foundation.
 */

//#include "pins_arduino.h"
#include "SPI.h"

static uint8_t SPI_initialized;// = 0;
static uint8_t SPI_interruptMode;// = 0;
static uint8_t SPI_interruptMask;// = 0;
static uint8_t SPI_interruptSave;// = 0;
#ifdef SPI_TRANSACTION_MISMATCH_LED
uint8_t SPI_inTransactionFlag;// = 0;
#endif


// from 1.8.0
void SPI_begin(void)
{
  uint8_t port;
  uint8_t bit;
  volatile uint8_t *reg;

  BEGIN_CRITICAL	// Protect from a scheduler and prevent transactionBegin
  if (!SPI_initialized) {
    // Set SS to high so a connected chip will be "deselected" by default
    port = digitalPinToPort(SS);
    bit = digitalPinToBitMask(SS);
    reg = portModeRegister(port);

    // if the SS pin is not already configured as an output
    // then set it high (to enable the internal pull-up resistor)
    if(!(*reg & bit)){
      digitalWrite(SS, HIGH);
    }

    // When the SS pin is set as OUTPUT, it can be used as
    // a general purpose output port (it doesn't influence
    // SPI operations).
    pinMode(SS, OUTPUT);

    // using software slave select frees the SS pins to be used as a general
    // IO pin without interfering with the SPI logic.
    SPI->CR2 = SPI_CR2_SSI | SPI_CR2_SSM;

    // Warning: if the SS pin ever becomes a LOW INPUT then SPI
    // automatically switches to Slave, so the data direction of
    // the SS pin MUST be kept as OUTPUT.
    SPI->CR1 |= SPI_CR1_MSTR | SPI_CR1_SPE;

    // Set direction register for SCK and MOSI pin.
    // MISO pin automatically overrides to INPUT.
    // By doing this AFTER enabling SPI, we avoid accidentally
    // clocking in a single bit since the lines go directly
    // from "input" to SPI control.
    // http://code.google.com/p/arduino/issues/detail?id=888
    pinMode(SCK, OUTPUT);
    pinMode(MOSI, OUTPUT);
  }
  SPI_initialized++; // reference count
  END_CRITICAL
}


#ifdef DONT_USE_ASSEMBLER
void SPI_transfer_s(void *buf, size_t count) {
    uint8_t *p;
    uint8_t in,out;

    if (count == 0) return;
    p = (uint8_t *)buf;
    SPI->DR = *p;			// start sending the first byte
    while (--count > 0) {
      out = *(p + 1);
      while ((SPI->SR & SPI_SR_TXE)==0) ; // wait for transmit buffer empty
	  SPI->DR = out;		// second byte into transmit buffer
      while ((SPI->SR & SPI_SR_RXNE)==0) ; // wait for first received byte
      in = SPI->DR;
      *p++ = in;
    }
    while ((SPI->SR & SPI_SR_RXNE)==0) ; // wait for last received byte
    *p = SPI->DR;
  }
#else
/*
 * to allow for an (almost) continues transfer the the following data byte
 * is written to TX before the received byte is read. So we have to make
 * sure to read the RX buffer within less than 16 clock cycles from the
 * moment of writing the TX data. This requires an interrupt block.
 *
 * pseudocode:

        init x,y
        dr = *x
        while (--y)
        {
                wait for txe, dr=x[1]
                wait for rxne, *x = dr
                x++
        }
        wait for rxne, *x = dr

 */
#if 0
void SPI_transfer_asm(void *buf, size_t count) {
	(void) buf;		// dummy code to avoid unreferenced
	(void) count;		// function argument warning
__asm
	ldw	y,(5,SP)
	jreq	$00001
	ldw	x,(3,SP)
	ld	a,(x)		; SPI->DR = *p
	ld	0x5204,a
$00002:
	decw	y		; while (--count>0) {
	jreq	$00003
	ld	a,(1,x)		;   out = *(p+1)
	btjf	0x5203,#1,.	;   while (!(SPI->SR & SPI_SR_TXE));
	sim			;   make sure to avoid a possible
				;   race condition to not miss the
				;   received data
	ld	0x5204,a	;   SPI->DR = out
	btjf	0x5203,#0,.	;   while (!(SPI->SR & SPI_SR_RXNE));
	ld	a,0x5204	;   in = SPI->DR
	rim
	ld	(x),a		;   *p++ = in
	incw	x
	jra	$00002		; } // while
$00003:
	btjf	0x5203,#0,.	; while (!(SPI->SR & SPI_SR_RXNE));
	ld	a,0x5204	; *p = SPI->DR
	ld	(x),a
$00001:
__endasm;
}
#endif

#if 1
void SPI_transfer_asm(void *buf, size_t count) {
	(void) buf;		// dummy code to avoid unreferenced
	(void) count;		// function argument warning
__asm
	ldw	y,(5,SP)
	jreq	$00001
	ldw	x,(3,SP)
	ld	a,(x)		; SPI->DR = *p
	ld	0x5204,a
	jra	$00003		; start the while loop
$00002:
	ld	a,(1,x)		;   out = *(p+1)
	btjf	0x5203,#1,.	;   while (!(SPI->SR & SPI_SR_TXE));
	sim			;   make sure to avoid a possible
				;   race condition to not miss the
				;   received data
	ld	0x5204,a	;   SPI->DR = out
	btjf	0x5203,#0,.	;   while (!(SPI->SR & SPI_SR_RXNE));
	ld	a,0x5204	;   in = SPI->DR
	rim
	ld	(x),a		;   *p++ = in
	incw	x
$00003:
	decw	y
	jrne	$00002		; } // while
	btjf	0x5203,#0,.	; while (!(SPI->SR & SPI_SR_RXNE));
	ld	a,0x5204	; *p = SPI->DR
	ld	(x),a
$00001:
__endasm;
}
#endif

#endif

#if 0
// slightly simplyfied functon:
__asm
        ldw     y,#8
        ldw     x,#_buf
        ld      a,(x)           ; dr = *x
        ld      0x5204,a
$00030:                         ; while (--y) {
        decw    y
        jreq    $00031
        btjf    0x5203,#1,.     ;   while (!(SPI->SR & SPI_SR_TXE));
        ld      a,(1,x)
        ld      0x5204,a
        btjf    0x5203,#0,.     ;   while (!(SPI->SR & SPI_SR_RXNE));
        ld      a,0x5204        ;   *x = dr
        ld      (x),a
        incw    x               ;   x++
        jra     $00030          ; }
$00031:
        btjf    0x5203,#0,.     ; while (!(SPI->SR & SPI_SR_RXNE));
        ld      a,0x5204        ; *x = dr
        ld      (x),a
__endasm;
#endif



void SPI_end(void) {
  BEGIN_CRITICAL
  // Decrease the reference counter
  if (SPI_initialized)
    SPI_initialized--;
  // If there are no more references disable SPI
  if (!SPI_initialized) {
    // follow the procedure recommended in RM0016, section 20.3.8:
    // Wait until RXNE = 1 to receive the last data
//    while ((SPI->SR&SPI_SR_RXNE)==0);
    // Wait until TXE = 1
    while ((SPI->SR&SPI_SR_TXE)==0);
    // Then wait until BSY = 0
    while (SPI->SR&SPI_SR_BSY);
    // Disable the SPI
    SPI->CR1 &= ~SPI_CR1_SPE;
    SPI_interruptMode = 0;
    #ifdef SPI_TRANSACTION_MISMATCH_LED
    SPI_inTransactionFlag = 0;
    #endif
  }
  END_CRITICAL
}

/*
void SPI_usingInterrupt(uint8_t interruptNumber)
{
  uint8_t mask = 0;
  uint8_t sreg = SREG;
  noInterrupts(); // Protect from a scheduler and prevent transactionBegin
  switch (interruptNumber) {
  #ifdef SPI_INT0_MASK
  case 0: mask = SPI_INT0_MASK; break;
  #endif
  #ifdef SPI_INT1_MASK
  case 1: mask = SPI_INT1_MASK; break;
  #endif
  #ifdef SPI_INT2_MASK
  case 2: mask = SPI_INT2_MASK; break;
  #endif
  #ifdef SPI_INT3_MASK
  case 3: mask = SPI_INT3_MASK; break;
  #endif
  #ifdef SPI_INT4_MASK
  case 4: mask = SPI_INT4_MASK; break;
  #endif
  #ifdef SPI_INT5_MASK
  case 5: mask = SPI_INT5_MASK; break;
  #endif
  #ifdef SPI_INT6_MASK
  case 6: mask = SPI_INT6_MASK; break;
  #endif
  #ifdef SPI_INT7_MASK
  case 7: mask = SPI_INT7_MASK; break;
  #endif
  default:
    interruptMode = 2;
    break;
  }
  interruptMask |= mask;
  if (!interruptMode)
    interruptMode = 1;
  SREG = sreg;
}
*/

/*
void SPI_notUsingInterrupt(uint8_t interruptNumber)
{
  // Once in mode 2 we can't go back to 0 without a proper reference count
  if (interruptMode == 2)
    return;
  uint8_t mask = 0;
  uint8_t sreg = SREG;
  noInterrupts(); // Protect from a scheduler and prevent transactionBegin
  switch (interruptNumber) {
  #ifdef SPI_INT0_MASK
  case 0: mask = SPI_INT0_MASK; break;
  #endif
  #ifdef SPI_INT1_MASK
  case 1: mask = SPI_INT1_MASK; break;
  #endif
  #ifdef SPI_INT2_MASK
  case 2: mask = SPI_INT2_MASK; break;
  #endif
  #ifdef SPI_INT3_MASK
  case 3: mask = SPI_INT3_MASK; break;
  #endif
  #ifdef SPI_INT4_MASK
  case 4: mask = SPI_INT4_MASK; break;
  #endif
  #ifdef SPI_INT5_MASK
  case 5: mask = SPI_INT5_MASK; break;
  #endif
  #ifdef SPI_INT6_MASK
  case 6: mask = SPI_INT6_MASK; break;
  #endif
  #ifdef SPI_INT7_MASK
  case 7: mask = SPI_INT7_MASK; break;
  #endif
  default:
    break;
    // this case can't be reached
  }
  interruptMask &= ~mask;
  if (!interruptMask)
    interruptMode = 0;
  SREG = sreg;
}
*/
