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

#ifndef _BV
#define _BV(X) (1<<(X))
#endif


#define NUM_DIGITAL_PINS            38
#define NUM_ANALOG_INPUTS           10
#define analogInputToDigitalPin(p)  ((p < NUM_ANALOG_INPUTS) ? (p) + (NUM_DIGITAL_PINS-NUM_ANALOG_INPUTS) : -1)


/* on the STM8S the logical pin numbers are really confusing instead
 * of beeing helpful. So maybe it is better to use these Portpin-Names
 * instead?
 */
enum portpin {
	PA6, /* CN1: 0-5 */
	PA5,
	PA4,
	PA3,
	PA2,
	PA1,
	PD7, /* CN4: 6-17 */
	PD6,
	PD5,
	PD4,
	PD3,
	PD2,
	PD1,
	PD0,
	PE0,
	PE1,
	PE2,
	PE3,
	PG1, /* CN2: 18-27 */
	PG0,
	PC7,
	PC6,
	PC5,
	PC4,
	PC3,
	PC2,
	PC1,
	PE5,
	PE6, /* CN2: 28-37, A0-A9 */
	PE7,
	PB0,
	PB1,
	PB2,
	PB3,
	PB4,
	PB5,
	PB6,
	PB7,
};



// PWM on pins 3,9-11,13,25-28
// #define digitalPinHasPWM(p)	( (p)==2 | (p)==5 | (p)==6 | (p)==12 )
 #define digitalPinHasPWM(p)	( (p)==3 \
 				| (p)==9 | (p)==10 | (p)==11 \
 				| (p)==13| \
				| ( (p)>=25 & (p)<=28 ) \
 )

/*
03 00011
09 01001
0a 01010
0b 01011
0d 01101
19 11001
1a 11010
1b 11011
1c 11100
*/

#define PIN_SPI_SS    (PE5)	// 27
#define PIN_SPI_MOSI  (PC6)	// 21
#define PIN_SPI_MISO  (PC7)	// 20
#define PIN_SPI_SCK   (PC5)	// 22

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

#define PIN_WIRE_SDA        (PE2)	// 16
#define PIN_WIRE_SCL        (PE1)	// 15

/* SDCC workaround
static const uint8_t SDA = PIN_WIRE_SDA;
static const uint8_t SCL = PIN_WIRE_SCL;
*/
#define	SDA	PIN_WIRE_SDA
#define	SCL	PIN_WIRE_SCL

#define PIN_LED_BUILTIN (PD0)	// sduino: pin for the buildin LED, pin 13
#define PIN_TX	(PD5)		// sduino: pin for TX line, pin 8
#define PIN_RX	(PD6)		// sduino: pin for RX line, pin 7

#define LED_BUILTIN (PD0)	// pin for the buildin LED, pin 13

#define PIN_A0   (PE6)		// 28, Ain9
#define PIN_A1   (PE7)		// 29, Ain8
#define PIN_A2   (PB0)		// 30, Ain0
#define PIN_A3   (PB1)		// 31, Ain1
#define PIN_A4   (PB2)		// 32, Ain2
#define PIN_A5   (PB3)		// 33, Ain3
#define PIN_A6   (PB4)		// 34, Ain4
#define PIN_A7   (PB5)		// 35, Ain5
#define PIN_A8   (PB6)		// 36, Ain6
#define PIN_A9   (PB7)		// 37, Ain7

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
#define	A5	PIN_A5
#define	A6	PIN_A6
#define	A7	PIN_A7
#define	A8	PIN_A8
#define	A9	PIN_A9

//#define NO_ANALOG	0xff

// map the logical pin numbers to the physical ADC channels:
// pin 28,29  -> channel 9,8 (reverse order!)
// pin 30..38 -> channel 0-7
// smaller numbers are not modified but used as channel numbers directly.
#define analogPinToChannel(P) ( (P)>=30 ? (P)-30 : ( \
				(P)>=28 ? 37-(P) : \
					  (P) \
				))


/*FIXME
#define digitalPinToPCICR(p)    (((p) >= 0 && (p) <= 21) ? (&PCICR) : ((uint8_t *)0))
#define digitalPinToPCICRbit(p) (((p) <= 7) ? 2 : (((p) <= 13) ? 0 : 1))
#define digitalPinToPCMSK(p)    (((p) <= 7) ? (&PCMSK2) : (((p) <= 13) ? (&PCMSK0) : (((p) <= 21) ? (&PCMSK1) : ((uint8_t *)0))))
#define digitalPinToPCMSKbit(p) (((p) <= 7) ? (p) : (((p) <= 13) ? ((p) - 8) : ((p) - 14)))

#define digitalPinToInterrupt(p)  ((p) == 2 ? 0 : ((p) == 3 ? 1 : NOT_AN_INTERRUPT))
*/

#ifdef ARDUINO_MAIN

// STM8S105C6 Discovery board

// Pin mapping for the four connectors CN1..CN4:
//
//
//			 RX          LED SCL
//		 Pin	  7   9~ 11~ 13~ 15  17
//		 Port	 PD6 PD4 PD2 PD0 PE1 PE3
//			+--------       --------+
//		 CN4	|11   9   7   5   3   1 |
//		 CN4	|12  10   8   6   4   2 |
//			+-----------------------+
//		 Port    PD7 PD5 PD3 PD1 PE0 PE2
//		 Pin	  6   8  10~ 12  14  16
//		             TX              SDA
//
// Pin Port   CN1   Port Pin		Pin Port   CN2   Port Pin
//          +-----+				 +-----+
//  -  NRST |*1  2| PA1   5		18  PG1  |12 11| PG0  19	
//  4  PA2  | 3  4| VssIO -	   MISO 20  PC7  |10  9| PC6  21 MOSI
//  -  Vss    5  6| Vcap  -		 - VddIO | 8  7  VssIO -
//  -  Vdd    7  8| VssIO -	    SCK 22  PC5  | 6  5  PC4  23~
//  3~ PA3  | 9 10| PA4   2		24~ PC3  | 4  3| PC2  25~
//  1  PA5  |11 12| PA6   0		26~ PC1  | 2 *1| PE5  27 SS
//          +-----+				 +-----+
//
//			     A8  A6  A4  A2  A0
//		 Pin	  -  36  34  32  30  28
//		 Port	 VssAPB6 PB4 PB2 PB0 PE6
//			+-----------------------+
//		 CN3	| 2   4   6   8  10  12 |
//		 CN3	| 1   3   5   7   9  11 |
//			+--------       --------+
//		 Port	 VddAPB7 PB5 PB3 PB1 PE7
//		 Pin	  -  37  35  33  31  29
//			     A9  A7  A5  A3  A1

// these arrays map port names (e.g. port B) to the
// appropriate addresses for various functions (e.g. reading
// and writing)
const uint16_t PROGMEM port_to_mode_PGM[] = {
	NOT_A_PORT,
	GPIOA_BaseAddress+2,
	GPIOB_BaseAddress+2,
	GPIOC_BaseAddress+2,
	GPIOD_BaseAddress+2,
	GPIOE_BaseAddress+2,
	GPIOG_BaseAddress+2,
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
	GPIOE_BaseAddress,
	GPIOG_BaseAddress,
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
	GPIOE_BaseAddress+1,
	GPIOG_BaseAddress+1,
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
	PA,
	PA,
	PA,
	PD, /* 6 */
	PD,
	PD,
	PD,
	PD,
	PD,
	PD,
	PD,
	PE, /* 14 */
	PE,
	PE,
	PE,
	PG, /* 18 */
	PG,
	PC, /* 20 */
	PC,
	PC,
	PC,
	PC,
	PC,
	PC,
	PE, /* 27 */
	PE,
	PE,
	PB, /* 30 */
	PB,
	PB,
	PB,
	PB,
	PB,
	PB,
	PB, /* 37 */
};

const uint8_t PROGMEM digital_pin_to_bit_mask_PGM[] = {
	_BV(6), /* 0, port A */
	_BV(5),
	_BV(4),
	_BV(3),
	_BV(2),
	_BV(1),
	_BV(7), /* 6, port D */
	_BV(6),
	_BV(5),
	_BV(4),
	_BV(3),
	_BV(2),
	_BV(1),
	_BV(0),
	_BV(0), /* 14, port E */
	_BV(1),
	_BV(2),
	_BV(3),
	_BV(1), /* 18, port G */
	_BV(0),
	_BV(7), /* 20, port C */
	_BV(6),
	_BV(5),
	_BV(4),
	_BV(3),
	_BV(2),
	_BV(1),
	_BV(5), /* 27, port E */
	_BV(6),
	_BV(7),
	_BV(0), /* 30, port B */
	_BV(1),
	_BV(2),
	_BV(3),
	_BV(4),
	_BV(5),
	_BV(6),
	_BV(7),
};

const uint8_t PROGMEM digital_pin_to_timer_PGM[] = {
	NOT_ON_TIMER,
	NOT_ON_TIMER,
	NOT_ON_TIMER,
	TIMER23,	// 3
	NOT_ON_TIMER,
	NOT_ON_TIMER,
	NOT_ON_TIMER,
	NOT_ON_TIMER,
	NOT_ON_TIMER,
	TIMER21,	// 9
	TIMER22,	// 10
	TIMER31,	// 11
	NOT_ON_TIMER,
	TIMER32,	// 13
	NOT_ON_TIMER,
	NOT_ON_TIMER,
	NOT_ON_TIMER,
	NOT_ON_TIMER,
	NOT_ON_TIMER,
	NOT_ON_TIMER,
	NOT_ON_TIMER,
	NOT_ON_TIMER,
	NOT_ON_TIMER,
	TIMER14,	// 23
	TIMER13,	// 24
	TIMER12,	// 25
	TIMER11,	// 26
	NOT_ON_TIMER,
	NOT_ON_TIMER,
	NOT_ON_TIMER,
	NOT_ON_TIMER,
	NOT_ON_TIMER,
	NOT_ON_TIMER,
	NOT_ON_TIMER,
	NOT_ON_TIMER,
	NOT_ON_TIMER,
	NOT_ON_TIMER,
	NOT_ON_TIMER,
};

#endif

#define NEED_TIMER_11_12
#define NEED_TIMER_23
#define NEED_TIMER_31_32

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
