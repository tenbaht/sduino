/*
 * Copyright (c) 2010 by Cristian Maglie <c.maglie@arduino.cc>
 * Copyright (c) 2014 by Paul Stoffregen <paul@pjrc.com> (Transaction API)
 * Copyright (c) 2014 by Matthijs Kooijman <matthijs@stdin.nl> (SPISettings AVR)
 * Copyright (c) 2014 by Andrew J. Kroll <xxxajk@gmail.com> (atomicity fixes)
 * Copyright (c) 2016 by Michael Mayer <michael-mayer@gmx.de> (c version for STM8 port)
 * SPI Master library for arduino.
 *
 * This file is free software; you can redistribute it and/or modify
 * it under the terms of either the GNU General Public License version 2
 * or the GNU Lesser General Public License version 2.1, both as
 * published by the Free Software Foundation.
 */

#ifndef _SPI_H_INCLUDED
#define _SPI_H_INCLUDED

#include <wiring_private.h>
//#include <Arduino.h>

// SPI_HAS_TRANSACTION means SPI has beginTransaction(), endTransaction(),
// usingInterrupt(), and SPISetting(clock, bitOrder, dataMode)
#define SPI_HAS_TRANSACTION 1

// SPI_HAS_NOTUSINGINTERRUPT means that SPI has notUsingInterrupt() method
#define SPI_HAS_NOTUSINGINTERRUPT 1

// SPI_ATOMIC_VERSION means that SPI has atomicity fixes and what version.
// This way when there is a bug fix you can check this define to alert users
// of your code if it uses better version of this library.
// This also implies everything that SPI_HAS_TRANSACTION as documented above is
// available too.
#define SPI_ATOMIC_VERSION 1

// Uncomment this line to add detection of mismatched begin/end transactions.
// A mismatch occurs if other libraries fail to use SPI.endTransaction() for
// each SPI.beginTransaction().  Connect an LED to this pin.  The LED will turn
// on if any mismatch is ever detected.
//#define SPI_TRANSACTION_MISMATCH_LED 5

#ifndef LSBFIRST
#define LSBFIRST 0
#endif
#ifndef MSBFIRST
#define MSBFIRST 1
#endif

#define SPI_CLOCK_DIV2		(0x00<<3)
#define SPI_CLOCK_DIV4		(0x01<<3)
#define SPI_CLOCK_DIV8		(0x02<<3)
#define SPI_CLOCK_DIV16		(0x03<<3)
#define SPI_CLOCK_DIV32		(0x04<<3)
#define SPI_CLOCK_DIV64 	(0x05<<3)
#define SPI_CLOCK_DIV128	(0x06<<3)
#define SPI_CLOCK_DIV256	(0x07<<3)

#define SPI_MODE0 0x00
#define SPI_MODE1 0x01
#define SPI_MODE2 0x02
#define SPI_MODE3 0x03

#define SPI_MODE_MASK	(3<<0)	// CPOL = bit 1, CPHA = bit 0 on SPI_CR1
#define SPI_CLOCK_MASK	(7<<3)	// BR[2:0] is bit [5:3] on SPI_CR1
#define SPI_BITORDER_MASK (1<<7)

// instead of a SPISettings class object just use a 8 bit integer value.
// bit 0..1: dataMode
// bit 3..5: clock divider
// bit 7:    bitOrder
//FIXME: it would be more efficient to change the definitions for bitOrder to
// values 0x00 and 0x80 to avoid shifting here. But it
// is not clear if different values for MSBFIRST would influence other code.
//FIXME: this is always inline. Efficient for constant values, terrible for
// variables.
#define SPISettings(C,O,M) ((((O==LSBFIRST)?0x80:0)|M)|( \
    F_CPU/2<=C ? SPI_CLOCK_DIV2 : (\
    F_CPU/4<=C ? SPI_CLOCK_DIV4 : (\
    F_CPU/8<=C ? SPI_CLOCK_DIV8 : (\
    F_CPU/16<=C ? SPI_CLOCK_DIV16 : (\
    F_CPU/32<=C ? SPI_CLOCK_DIV32 : (\
    F_CPU/64<=C ? SPI_CLOCK_DIV64 : (\
    F_CPU/128<=C ? SPI_CLOCK_DIV128 : (\
    SPI_CLOCK_DIV256 )))))))))


  // Initialize the SPI library
void SPI_begin(void);

  // If SPI is used from within an interrupt, this function registers
  // that interrupt with the SPI library, so beginTransaction() can
  // prevent conflicts.  The input interruptNumber is the number used
  // with attachInterrupt.  If SPI is used from a different interrupt
  // (eg, a timer), interruptNumber should be 255.
void SPI_usingInterrupt(uint8_t interruptNumber);
  // And this does the opposite.
void SPI_notUsingInterrupt(uint8_t interruptNumber);
  // Note: the usingInterrupt and notUsingInterrupt functions should
  // not to be called from ISR context or inside a transaction.
  // For details see:
  // https://github.com/arduino/Arduino/pull/2381
  // https://github.com/arduino/Arduino/pull/2449

  // Before using SPI.transfer() or asserting chip select pins,
  // this function is used to gain exclusive access to the SPI bus
  // and configure the correct settings.
inline void SPI_beginTransaction(uint8_t settings) {
/*
    if (interruptMode > 0) {
      uint8_t sreg = SREG;
      noInterrupts();

      #ifdef SPI_AVR_EIMSK
      if (interruptMode == 1) {
        interruptSave = SPI_AVR_EIMSK;
        SPI_AVR_EIMSK &= ~interruptMask;
        SREG = sreg;
      } else
      #endif
      {
        interruptSave = sreg;
      }
    }
*/
    #ifdef SPI_TRANSACTION_MISMATCH_LED
    if (SPI_inTransactionFlag) {
      pinMode(SPI_TRANSACTION_MISMATCH_LED, OUTPUT);
      digitalWrite(SPI_TRANSACTION_MISMATCH_LED, HIGH);
    }
    SPI_inTransactionFlag = 1;
    #endif

    SPI->CR1 = settings | SPI_CR1_MSTR | SPI_CR1_SPE;
  }

// Write to the SPI bus (MOSI pin) and also receive (MISO pin)
inline uint8_t SPI_transfer(uint8_t data)
{
    SPI->DR = data;
    /*
     * The following NOP introduces a small delay that can prevent the wait
     * loop form iterating when running at the maximum speed. This gives
     * about 10% more speed, even if it seems counter-intuitive. At lower
     * speeds it is unnoticed.
     */
    __asm__("nop");
    while (!(SPI->SR & SPI_SR_RXNE)) ; // wait
    return SPI->DR;
}

/*
  inline static uint16_t transfer16(uint16_t data) {
    union { uint16_t val; struct { uint8_t lsb; uint8_t msb; }; } in, out;
    in.val = data;
    if (!(SPCR & _BV(DORD))) {
      SPDR = in.msb;
      asm volatile("nop"); // See transfer(uint8_t) function
      while (!(SPSR & _BV(SPIF))) ;
      out.msb = SPDR;
      SPDR = in.lsb;
      asm volatile("nop");
      while (!(SPSR & _BV(SPIF))) ;
      out.lsb = SPDR;
    } else {
      SPDR = in.lsb;
      asm volatile("nop");
      while (!(SPSR & _BV(SPIF))) ;
      out.lsb = SPDR;
      SPDR = in.msb;
      asm volatile("nop");
      while (!(SPSR & _BV(SPIF))) ;
      out.msb = SPDR;
    }
    return out.val;
  }
*/

#define SPIv ((volatile SPI_TypeDef *) SPI_BaseAddress)
// transmit a data block over SPI bus
void SPI_transfer_s(void *buf, size_t count);
void SPI_transfer_asm(void *buf, size_t count);
/*
inline void SPI_transfer_asm(void *buf, size_t count) {
	__asm
	tnzw	y			; if (count==0) return
	jreq	$00001
	ld		a,(x)		; SPI->DR = *p
	ld		0x5204,a
$00002:
	decw	y			; while (--count>0) {
	jreq	$00003
	ld		a,(1,x)		; out = *(p+1)
$00004:
	btjf	0x5203,#1,$00004	; while (!(SPI->SR & SPI_SR_TXE));
	ld		0x5204,a	; SPI->DR = out
$00005:
	btjf	0x5203,#0,$00005	; while (!(SPI->SR & SPI_SR_RXNE));
	ld		a,0x5204	; in = SPI->DR
	ld		(x),a		; *p++ = in
	incw	x
$00003:					; } // while
	btjf	0x5203,#0,$00003	; while (!(SPI->SR & SPI_SR_RXNE));
	ld		a,0x5204	; *p = SPI->DR
	ld		(x),a
$00001:
	__endasm;
}
*/
inline void SPI_transfer_n(void *buf, size_t count) {
    uint8_t *p;
    uint8_t in,out;

    if (count == 0) return;
    p = (uint8_t *)buf;
    SPIv->DR = *p;			// start sending the first byte
    while (--count > 0) {
      out = *(p + 1);
//      while (!(SPIv->SR & SPI_SR_TXE)) ; // wait for transmit buffer empty
      while (!(*((volatile uint8_t *)0x5203) & SPI_SR_TXE)) ; // wait for transmit buffer empty
	  SPI->DR = out;		// second byte into transmit buffer
      while ((SPIv->SR & SPI_SR_RXNE)==0) ; // wait for first received byte
      in = SPI->DR;
      *p++ = in;
    }
    while ((SPIv->SR & SPI_SR_RXNE)==0) ; // wait for last received byte
    *p = SPIv->DR;
  }

  // After performing a group of transfers and releasing the chip select
  // signal, this function allows others to access the SPI bus

inline void SPI_endTransaction(void) {
    #ifdef SPI_TRANSACTION_MISMATCH_LED
    if (!inTransactionFlag) {
      pinMode(SPI_TRANSACTION_MISMATCH_LED, OUTPUT);
      digitalWrite(SPI_TRANSACTION_MISMATCH_LED, HIGH);
    }
    inTransactionFlag = 0;
    #endif
/*
    if (interruptMode > 0) {
      #ifdef SPI_AVR_EIMSK
      uint8_t sreg = SREG;
      #endif
      noInterrupts();
      #ifdef SPI_AVR_EIMSK
      if (interruptMode == 1) {
        SPI_AVR_EIMSK = interruptSave;
        SREG = sreg;
      } else
      #endif
      {
        SREG = interruptSave;
      }
    }
*/
  }


// Disable the SPI bus
void SPI_end(void);

// This function is deprecated.  New applications should use
// beginTransaction() to configure SPI settings.
inline void SPI_setBitOrder(uint8_t bitOrder) {
    if (bitOrder == LSBFIRST) SPI->CR1 |= SPI_FIRSTBIT_LSB;
    else SPI->CR1 &= ~(SPI_FIRSTBIT_LSB);
}

// This function is deprecated.  New applications should use
// beginTransaction() to configure SPI settings.
inline void SPI_setDataMode(uint8_t dataMode) {
    SPI->CR1 = (SPI->CR1 & ~SPI_MODE_MASK) | dataMode;
}

// This function is deprecated.  New applications should use
// beginTransaction() to configure SPI settings.
inline void SPI_setClockDivider(uint8_t clockDiv) {
    SPI->CR1 = (SPI->CR1 & ~SPI_CLOCK_MASK) | (clockDiv & SPI_CLOCK_MASK);
}

// These undocumented functions should not be used.  SPI.transfer()
// polls the hardware flag which is automatically cleared as the
// AVR responds to SPI's interrupt
inline void SPI_attachInterrupt(void) { SPI->ICR |= _BV(SPI_IT_RXNE&0xf); }
inline void SPI_detachInterrupt(void) { SPI->ICR &= ~_BV(SPI_IT_RXNE&0xf); }

/*
private:
  static uint8_t initialized;
  static uint8_t interruptMode; // 0=none, 1=mask, 2=global
  static uint8_t interruptMask; // which interrupts to mask
  static uint8_t interruptSave; // temp storage, to restore state
  #ifdef SPI_TRANSACTION_MISMATCH_LED
  static uint8_t inTransactionFlag;
  #endif
};
*/

#endif
