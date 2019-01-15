/*
  pins_arduino.h - Pin definition functions for Arduino
  Part of Arduino - http://www.arduino.cc/

  Copyright (c) 2007 David A. Mellis

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General
  Public License along with this library; if not, write to the
  Free Software Foundation, Inc., 59 Temple Place, Suite 330,
  Boston, MA  02111-1307  USA
*/

#ifndef Pins_Arduino_h
#define Pins_Arduino_h

//#include <avr/pgmspace.h>

#ifndef _BV
#define _BV(X) (1<<(X))
#endif


#define NUM_DIGITAL_PINS            16
#define NUM_ANALOG_INPUTS           5


/* on the STM8S the logical pin numbers are really confusing instead
 * of beeing helpful. So maybe it is better to use these Portpin-Names
 * instead?
 */
enum portpin {
	PA1, /* 0 */
	PA2,
	PA3,
	PB5, /* 3 */
	PB4,
	PC3, /* 5 */
	PC4,
	PC5,
	PC6,
	PC7,
	PD1, /* 10 */
	PD2,
	PD3,
	PD4,
	PD5,
	PD6  /* 15 */
};



/* SDCC problem: this const table won't be optimized away but will show
 * up in every single object file.
static const uint8_t PROGMEM analogInputToDigitalPinMap[5]={6,11,12,14,15};
#define analogInputToDigitalPin(p)  (analogInputToDigitalPinMap[p])
*/

/* SDCC workaround: a crude compile time look up table.
 * This way it does get optimized to a hard constant load when used with a
 * const value. It is extremy ineffizient for run time accesses though.
 */

#define analogInputToDigitalPin(p)  (	(p<0)?-1:(\
					(p<1)?PIN_A0:(\
					(p<2)?PIN_A1:(\
					(p<3)?PIN_A2:(\
					(p<4)?PIN_A3:(\
					(p<5)?PIN_A4:(\
					-1)))))))



#ifdef SUPPORT_ALTERNATE_MAPPINGS
 // using alternate functions adds 3 more PWM pins, total of 7 PWM pins
 #define digitalPinHasPWM(p)	( (p)==2 | ((p)>=5&(p)<=9) | (p)==12 )
#else
 // standard case: only 4 regular PWM pins
 #define digitalPinHasPWM(p)	( (p)==2 | (p)==5 | (p)==6 | (p)==12 )
#endif

#define PIN_SPI_SS    (PA3)	// 2
#define PIN_SPI_MOSI  (PC6)	// 8
#define PIN_SPI_MISO  (PC7)	// 9
#define PIN_SPI_SCK   (PC5)	// 7

/* SDCC workaround: These const variables wouldn't be replaced by hard
 * constant loads. So use defines instead.
static const uint8_t SS   = PIN_SPI_SS;
static const uint8_t MOSI = PIN_SPI_MOSI;
static const uint8_t MISO = PIN_SPI_MISO;
static const uint8_t SCK  = PIN_SPI_SCK;
*/
#define SS	PIN_SPI_SS
#define	MOSI	PIN_SPI_MOSI
#define	MISO	PIN_SPI_MISO
#define	SCK	PIN_SPI_SCK

#define PIN_WIRE_SDA        (PB5)	// 3
#define PIN_WIRE_SCL        (PB4)	// 4

/* SDCC workaround
static const uint8_t SDA = PIN_WIRE_SDA;
static const uint8_t SCL = PIN_WIRE_SCL;
*/
#define	SDA	PIN_WIRE_SDA
#define	SCL	PIN_WIRE_SCL

#define PIN_LED_BUILTIN (PB5)	// sduino: pin for the buildin LED, pin 3
#define PIN_TX	(PD5)		// sduino: pin for TX line
#define PIN_RX	(PD6)		// sduino: pin for RX line

#define LED_BUILTIN (PB5)	// pin for the buildin LED, pin 3

#define PIN_A0   (PC4)		//  6, Ain2
#define PIN_A1   (PD2)		// 11, Ain3
#define PIN_A2   (PD3)		// 12, Ain4
#define PIN_A3   (PD5)		// 14, Ain5
#define PIN_A4   (PD6)		// 15, Ain6

/* SDCC workaround
static const uint8_t A0 = PIN_A0;
static const uint8_t A1 = PIN_A1;
static const uint8_t A2 = PIN_A2;
static const uint8_t A3 = PIN_A3;
static const uint8_t A4 = PIN_A4;
*/
#define	A0	PIN_A0
#define	A1	PIN_A1
#define	A2	PIN_A2
#define	A3	PIN_A3
#define	A4	PIN_A4

// Distinguish between ADC channel number and digital pin number.
// Note that for value 6 both ranges overlap and it is used a pin number.
//
// values 0..5: ADC channel number, no conversion
// values 6..15: digital pin numbers, convert to ADC channel number
#define analogPinToChannel(P)	( (P)<6 ? (P) : digitalPinToAnalogChannelMap[(P-6)] )
extern const uint8_t digitalPinToAnalogChannelMap[];

/*FIXME
#define digitalPinToPCICR(p)    (((p) >= 0 && (p) <= 21) ? (&PCICR) : ((uint8_t *)0))
#define digitalPinToPCICRbit(p) (((p) <= 7) ? 2 : (((p) <= 13) ? 0 : 1))
#define digitalPinToPCMSK(p)    (((p) <= 7) ? (&PCMSK2) : (((p) <= 13) ? (&PCMSK0) : (((p) <= 21) ? (&PCMSK1) : ((uint8_t *)0))))
#define digitalPinToPCMSKbit(p) (((p) <= 7) ? (p) : (((p) <= 13) ? ((p) - 8) : ((p) - 14)))

#define digitalPinToInterrupt(p)  ((p) == 2 ? 0 : ((p) == 3 ? 1 : NOT_AN_INTERRUPT))
*/

#ifdef ARDUINO_MAIN

// STM8S103F3 breakout board
//
//                       +-\/-+
// PWM      (D 13) PD4  1|    |20  PD3 (D 12, AI 2) PWM
// TX (D 14, AI 3) PD5  2|    |19  PD2 (D 11, AI 1)
// RX (D 15, AI 4) PD6  3|    |18  PD1 (D 10)
//                NRST  4|    |17  PC7 (D 9)   MISO/PWM+
//           (D 0) PA1  5|    |16  PC6 (D 8)   MOSI/PWM+
//           (D 1) PA2  6|    |15  PC5 (D 7)    SCK
//                 GND  7|    |14  PC4 (D 6, AI 0)  PWM
//                Vcap  8|    |13  PC3 (D 5)        PWM
//                 Vdd  9|    |12  PB4 (D 4)        SCL
// PWM       (D 2) PA3 10|    |11  PB5 (D 3)   LED, SDA
//                       +----+
//
// (PWM+ indicates the additional PWM pins on alternate
// function pins)

// these arrays map port names (e.g. port B) to the
// appropriate addresses for various functions (e.g. reading
// and writing)
const uint16_t PROGMEM port_to_mode_PGM[] = {
	NOT_A_PORT,
	GPIOA_BaseAddress+2,
	GPIOB_BaseAddress+2,
	GPIOC_BaseAddress+2,
	GPIOD_BaseAddress+2,
/*
	(uint16_t) &GPIOA->DDR,
	(uint16_t) &GPIOB->DDR,
	(uint16_t) &GPIOC->DDR,
	(uint16_t) &GPIOD->DDR,
*/
};

const uint16_t PROGMEM port_to_output_PGM[] = {
	NOT_A_PORT,
	GPIOA_BaseAddress,
	GPIOB_BaseAddress,
	GPIOC_BaseAddress,
	GPIOD_BaseAddress,
/*
	(uint16_t) &GPIOA->ODR,
	(uint16_t) &GPIOB->ODR,
	(uint16_t) &GPIOC->ODR,
	(uint16_t) &GPIOD->ODR,
*/
};

const uint16_t PROGMEM port_to_input_PGM[] = {
	NOT_A_PORT,
	GPIOA_BaseAddress+1,
	GPIOB_BaseAddress+1,
	GPIOC_BaseAddress+1,
	GPIOD_BaseAddress+1,
/*
	(uint16_t) &GPIOA->IDR,
	(uint16_t) &GPIOB->IDR,
	(uint16_t) &GPIOC->IDR,
	(uint16_t) &GPIOD->IDR,
*/
};

const uint8_t PROGMEM digital_pin_to_port_PGM[] = {
	PA, /* 0 */
	PA,
	PA,
	PB, /* 3 */
	PB,
	PC, /* 5 */
	PC,
	PC,
	PC,
	PC,
	PD, /* 10 */
	PD,
	PD,
	PD,
	PD,
	PD, /* 15 */
};

const uint8_t PROGMEM digital_pin_to_bit_mask_PGM[] = {
	_BV(1), /* 0, port A */
	_BV(2),
	_BV(3),
	_BV(5), /* 3, port B */
	_BV(4),
	_BV(3), /* 5, port C */
	_BV(4),
	_BV(5),
	_BV(6),
	_BV(7),
	_BV(1), /* 10, port D */
	_BV(2),
	_BV(3),
	_BV(4),
	_BV(5),
	_BV(6),
};

const uint8_t PROGMEM digital_pin_to_timer_PGM[] = {
	NOT_ON_TIMER,
	NOT_ON_TIMER,
	TIMER23,	// 2
	NOT_ON_TIMER,
	NOT_ON_TIMER,
	TIMER13,	// 5
	TIMER14,	// 6
#ifdef SUPPORT_ALTERNATE_MAPPINGS
	TIMER21,	// 7, TIMER21,	duplicate from PD4/dig. 13
	TIMER11,	// 8
	TIMER12,	// 9
#else
	NOT_ON_TIMER,	// 7
	NOT_ON_TIMER,	// 8
	NOT_ON_TIMER,	// 9
#endif
	NOT_ON_TIMER,
	NOT_ON_TIMER,	// 11, TIMER23, duplicate from PA3/dig. 3
	TIMER22,	// 12
	TIMER21,	// 13
	NOT_ON_TIMER,
	NOT_ON_TIMER,
};

#define NO_ANALOG	0xff

const uint8_t digitalPinToAnalogChannelMap[] = {
	2,		// A0	D6	PC4	Ain2
	NO_ANALOG,	//	D7	PC5
	NO_ANALOG,	//	D8	PC6
	NO_ANALOG,	//	D9	PC7
	NO_ANALOG,	//	D10	PD1
	3,		// A1	D11	PD2	Ain3
	4,		// A2	D12	PD3	Ain4
	NO_ANALOG,	//	D13	PD4
	5,		// A3	D14	PD5	Ain5
	6		// A4	D15	PD6	Ain6
};

#endif /* ARDUINO_MAIN */

#define NEED_TIMER_23
#ifdef SUPPORT_ALTERNATE_MAPPINGS
# define NEED_TIMER_11_12
#endif


// These serial port names are intended to allow libraries and architecture-neutral
// sketches to automatically default to the correct port name for a particular type
// of use.  For example, a GPS module would normally connect to SERIAL_PORT_HARDWARE_OPEN,
// the first hardware serial port whose RX/TX pins are not dedicated to another use.
//
// SERIAL_PORT_MONITOR        Port which normally prints to the Arduino Serial Monitor
//
// SERIAL_PORT_USBVIRTUAL     Port which is USB virtual serial
//
// SERIAL_PORT_LINUXBRIDGE    Port which connects to a Linux system via Bridge library
//
// SERIAL_PORT_HARDWARE       Hardware serial port, physical RX & TX pins.
//
// SERIAL_PORT_HARDWARE_OPEN  Hardware serial ports which are open for use.  Their RX & TX
//                            pins are NOT connected to anything by default.
#define SERIAL_PORT_MONITOR   Serial
#define SERIAL_PORT_HARDWARE  Serial

#endif
