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


#define NUM_DIGITAL_PINS            25
#define NUM_ANALOG_INPUTS           9
#define analogInputToDigitalPin(p)  ((p < NUM_ANALOG_INPUTS) ? (p) + (NUM_DIGITAL_PINS-NUM_ANALOG_INPUTS) : -1)


/* strict geometrical pin mapping for the STM8S105K4T6 breakout board.
 *
 * This pin mapping is very intuitive when working on a break board, but
 * all pin numbers are totally different than on any other board. So all
 * existing sketches will require adopted pin numbers.
 *
 * Potential pitfall: There is an overlap of the number ranges of analog
 * channel numbers and digital pin numbers for pins 3..5. Values of 3..5
 * will be treated as channel numbers, not as digital pin numbers.
 */
enum portpin {
	PA1, /* 0	OSC IN			*/
	PA2, /* 1	OSC OUT			*/
	PF4, /* 2	AIN12			*/
	PB5, /* 3	AIN5 / [I2C_SDA]	*/
	PB4, /* 4	AIN4 / [I2C_SCL]	*/
	PB3, /* 5	AIN3 / [TIM1_ETR]	*/
	PB2, /* 6	AIN2 / [TIM2_CH3N]	*/
	PB1, /* 7	AIN1 / [TIM1_CH2N]	*/
	PB0, /* 8	AIN0 / [TIM1_CH1N]	*/
	PE5, /* 9	SPI_NSS			*/
	PC1, /* 10~	TIM1_CH1 / UART2_CK	*/
	PC2, /* 11~	TIM1_CH2		*/
	PD7, /* 12	TLI / [TIM1_CH4]	*/
	PD6, /* 13	UART2_RX		*/
	PD5, /* 14	UART2_TX		*/
	PD4, /* 15~	TIM2_CH1 / [BEEP]	*/
	PD3, /* 16~	TIM2_CH2 / [ADC_ETR]	*/
	PD2, /* 17~	TIM3_CH1 / [TIM2_CH3]	*/
	PD1, /* 18	SWIM			*/
	PD0, /* 19~	TIM3_CH2 / [TIM1_BKIN] / [CLK_CCO] */
	PC7, /* 20	SPI_MISO		*/
	PC6, /* 21	SPI_MOSI		*/
	PC5, /* 22	SPI_SCK			*/
	PC4, /* 23~	TIM1_CH4		*/
	PC3, /* 24~	TIM1_CH3		*/
};



// PWM on pins 12-13,13,25-28
 // using alternate functions adds 3 more PWM pins, total of 7 PWM pins
#define digitalPinHasPWM(p)	( ((p)>=6&(p)<=9) | ((p)>=11&(p)<=13) | ((p)>=16&(p)<=18) | (p)==20 | (p)==24 | (p)==25 |)


#define PIN_SPI_SS    (PE5)	// 11
#define PIN_SPI_MOSI  (PC6)	// 15
#define PIN_SPI_MISO  (PC7)	// 16
#define PIN_SPI_SCK   (PC5)	// 14

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

#define PIN_LED_BUILTIN (PE5)	// sduino: pin for the buildin LED, pin 13
#define PIN_TX	(PD5)		// sduino: pin for TX line, pin 8
#define PIN_RX	(PD6)		// sduino: pin for RX line, pin 7

#define LED_BUILTIN (PE5)	// pin for the buildin LED, pin 13

// ADC (digital) pin numbers
// note that for A0..A2 these numbers overlap with the channel numbers
// and the values would be misstaken for an ADC channel number.
// This is a serious problem with the strict geometrical mapping of the pin
// numbers.
#define PIN_A0   (PB5)		// 3, Ain5
#define PIN_A1   (PB4)		// 4, Ain4
#define PIN_A2   (PB3)		// 5, Ain3
#define PIN_A3   (PB2)		// 6, Ain2
#define PIN_A4   (PB1)		// 7, Ain1
#define PIN_A5   (PB0)		// 8, Ain0

/* SDCC workaround
static const uint8_t A0 = PIN_A0;
static const uint8_t A1 = PIN_A1;
static const uint8_t A2 = PIN_A2;
static const uint8_t A3 = PIN_A3;
static const uint8_t A4 = PIN_A4;
*/
// ADC channel numbers
#define	A0	5		// Ain5
#define	A1	4		// Ain4
#define	A2	3		// Ain3
#define	A3	2		// Ain2
#define	A4	1		// Ain1
#define	A5	0		// Ain0

#define NO_ANALOG	0xff

// Distinguish between ADC channel number and digital pin number.
// Note that for values 3..5 both ranges overlap and it is used a channel
// number.
//
// values 0..5: ADC channel number, no conversion
// values 6..8: digital pin numbers, convert to ADC channel number
#define analogPinToChannel(P) (	(P)<=5 ? (P) : ( \
					(P)<=PIN_A5 ? PIN_A5-(P) : NO_ANALOG \
				))
extern const uint8_t digitalPinToAnalogChannelMap[];

/*FIXME
#define digitalPinToPCICR(p)    (((p) >= 0 && (p) <= 21) ? (&PCICR) : ((uint8_t *)0))
#define digitalPinToPCICRbit(p) (((p) <= 7) ? 2 : (((p) <= 13) ? 0 : 1))
#define digitalPinToPCMSK(p)    (((p) <= 7) ? (&PCMSK2) : (((p) <= 13) ? (&PCMSK0) : (((p) <= 21) ? (&PCMSK1) : ((uint8_t *)0))))
#define digitalPinToPCMSKbit(p) (((p) <= 7) ? (p) : (((p) <= 13) ? ((p) - 8) : ((p) - 14)))

#define digitalPinToInterrupt(p)  ((p) == 2 ? 0 : ((p) == 3 ? 1 : NOT_AN_INTERRUPT))
*/

#ifdef ARDUINO_MAIN

// STM8S105K5 breakout board

// STM8S103F3 breakout board
//
//                       +-oooo-+
//                NRST  1|      |30  PD7 (D 24)  PWM+
//           (D 0) PA1  2|      |29  PD6 (D 23)  RX
//           (D 1) PA2  3|      |27  PD5 (D 22)  TX
//           (D 2) PF4  4|      |27  PD4 (D 21~) PWM
// SDA  (D 3,AI 0) PB5  5|      |26  PD3 (D 20~) PWM
// SCL  (D 4,AI 1) PB4  6|      |25  PD2 (D 19~) PWM
//      (D 5,AI 2) PB3  7|      |24  PD1 (D 18)
// PWM+ (D 6,AI 3) PB2  8|      |23  PD0 (D 17~) PWM
// PWM+ (D 7,AI 4) PB1  9|      |22  PC7 (D 16)  MISO
// PWM+ (D 8,AI 5) PB0 10|      |21  PC6 (D 15)  MOSI
// LED NSS   (D 9) PE5 11|      |20  PC5 (D 14)  SCK
// PWM     (D 10~) PC1 12|      |19  PC4 (D 13~) PWM
// PWM     (D 11~) PC2 13|      |18  PC3 (D 12~) PWM
//                 3V3 14|      |17  5V
//                 GND 15|      |16  GND
//                       +-\__/-+
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
	GPIOE_BaseAddress+2,
	GPIOF_BaseAddress+2,
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
	GPIOF_BaseAddress,
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
	GPIOF_BaseAddress+1,
/*
	(uint16_t) &GPIOA->IDR,
	(uint16_t) &GPIOB->IDR,
	(uint16_t) &GPIOC->IDR,
	(uint16_t) &GPIOD->IDR,
*/
};

const uint8_t PROGMEM digital_pin_to_port_PGM[] = {
	PA,   /* 0 */
	PA,
	PF,   /* 2 */
	PB,   /* 3 */
	PB, 
	PB,
	PB,
	PB,
	PB,
	PE,  /* 9 */
	PC,  /* 10 */
	PC,
	PC,
	PC,
	PC,
	PC,
	PC,
	PD,  /* 15 */
	PD,
	PD,
	PD,
	PD,
	PD,
	PD,
	PD,  /* 24 */
};

const uint8_t PROGMEM digital_pin_to_bit_mask_PGM[] = {
	_BV(1), /* 0, port A */
	_BV(2),
	_BV(4), /* 2, port F */
	_BV(5), /* 3, port B */
	_BV(4),
	_BV(3),
	_BV(2),
	_BV(1),
	_BV(0), 
	_BV(5), /* 9, port E */ 
	_BV(1), /* 10, port C */
	_BV(2),
	_BV(3),
	_BV(4),
	_BV(5), 
	_BV(6),
	_BV(7),
	_BV(0), /* 17, port D */
	_BV(1),
	_BV(2),
	_BV(3),
	_BV(4),
	_BV(5),
	_BV(6),
	_BV(7),
};

const uint8_t PROGMEM digital_pin_to_timer_PGM[] = {
	NOT_ON_TIMER,	// 0
	NOT_ON_TIMER,	// 1
	NOT_ON_TIMER,	// 2
	NOT_ON_TIMER,	// 3
	NOT_ON_TIMER,	// 4
	NOT_ON_TIMER,	// 5
#ifdef SUPPORT_ALTERNATE_MAPPINGS
	TIMER13,	// 6
	TIMER12,	// 7
	TIMER11,	// 8
#else
	NOT_ON_TIMER,	// 6
	NOT_ON_TIMER,	// 7
	NOT_ON_TIMER,	// 8
#endif
	NOT_ON_TIMER,	// 9
	TIMER11,	// 10
	TIMER12,	// 11
	TIMER13,	// 12
	TIMER14,	// 13
	NOT_ON_TIMER,	// 14
	NOT_ON_TIMER,	// 15
	NOT_ON_TIMER,	// 16
	TIMER32,	// 17
	NOT_ON_TIMER,	// 18
	TIMER31,	// 19
	TIMER22,	// 20
	TIMER21,	// 21
	NOT_ON_TIMER,	// 22
	NOT_ON_TIMER,	// 23
#ifdef SUPPORT_ALTERNATE_MAPPINGS
	TIMER14,	// 24
#else
	NOT_ON_TIMER,	//24
#endif
};

#endif

#define NEED_TIMER_11_12
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
