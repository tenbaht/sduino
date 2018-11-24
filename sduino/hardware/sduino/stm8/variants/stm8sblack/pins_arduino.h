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
#define NUM_ANALOG_INPUTS           6
#define analogInputToDigitalPin(p)  ((p < NUM_ANALOG_INPUTS) ? 16+(p) : -1)


/* strict geometrical pin mapping for the STM8S105K4T6 breakout board.
 *
 * This pin mapping is very intuitive when working on a break board, but
 * all pin numbers are totally different than on any other board. So all
 * existing sketches will require adopted pin numbers.
 *
 */
enum portpin {
	PC3, /* 0~	TIM1_CH3		*/
	PC4, /* 1~	TIM1_CH4		*/
	PC5, /* 2	SPI_SCK			*/
	PC6, /* 3	SPI_MOSI		*/
	PC7, /* 4	SPI_MISO		*/
	PD0, /* 5~	TIM3_CH2 / [TIM1_BKIN] / [CLK_CCO] */
	PD1, /* 6	SWIM			*/
	PD2, /* 7~	TIM3_CH1 / [TIM2_CH3]	*/
	PD3, /* 8~	TIM2_CH2 / [ADC_ETR]	*/
	PD4, /* 9~	TIM2_CH1 / [BEEP]	*/
	PD5, /* 10	UART2_TX		*/
	PD6, /* 11	UART2_RX		*/
	PD7, /* 12	TLI / [TIM1_CH4]	*/
	PA1, /* 13	OSC IN			*/
	PA2, /* 14	OSC OUT			*/
	PF4, /* 15	AIN12			*/
	PB5, /* 16	AIN5 / [I2C_SDA]	*/
	PB4, /* 17	AIN4 / [I2C_SCL]	*/
	PB3, /* 18	AIN3 / [TIM1_ETR]	*/
	PB2, /* 19	AIN2 / [TIM2_CH3N]	*/
	PB1, /* 20	AIN1 / [TIM1_CH2N]	*/
	PB0, /* 21	AIN0 / [TIM1_CH1N]	*/
	PE5, /* 22	SPI_NSS			*/
	PC1, /* 23~	TIM1_CH1 / UART2_CK	*/
	PC2, /* 24~	TIM1_CH2		*/
};


// PWM on pins 0,1,5,7-9,23,24
//#define digitalPinHasPWM(p)	( ((p)>=6&(p)<=9) | ((p)>=11&(p)<=13) | ((p)>=16&(p)<=18) | (p)==20 | (p)==24 | (p)==25 |)
#define digitalPinHasPWM(p)	( ((p)<2) | ((p)==5) | ((p)>=7&(p)<=9) | ((p)==23) | ((p)==24) )

#define PIN_SPI_SS    (PE5)	// 22
#define PIN_SPI_MOSI  (PC6)	//  3
#define PIN_SPI_MISO  (PC7)	//  4
#define PIN_SPI_SCK   (PC5)	//  2

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

#define PIN_WIRE_SDA        (PB5)	// 17
#define PIN_WIRE_SCL        (PB4)	// 16

/* SDCC workaround
static const uint8_t SDA = PIN_WIRE_SDA;
static const uint8_t SCL = PIN_WIRE_SCL;
*/
#define	SDA	PIN_WIRE_SDA
#define	SCL	PIN_WIRE_SCL

#define PIN_LED_BUILTIN (PE5)	// sduino: pin for the buildin LED, pin 22
#define PIN_TX	(PD5)		// sduino: pin for TX line, pin 13
#define PIN_RX	(PD6)		// sduino: pin for RX line, pin 14

#define LED_BUILTIN (PE5)	// pin for the buildin LED, pin 22

// ADC (digital) pin numbers
#define PIN_A0   (PB5)		// 16, Ain5
#define PIN_A1   (PB4)		// 17, Ain4
#define PIN_A2   (PB3)		// 18, Ain3
#define PIN_A3   (PB2)		// 19, Ain2
#define PIN_A4   (PB1)		// 20, Ain1
#define PIN_A5   (PB0)		// 21, Ain0

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
//
// values 0..5: ADC channel number, no conversion
// values 16..21: digital pin numbers, convert to ADC channel number
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
//
//                       +-/--\-+
//                 GND  1|      |30  GND
//                  5V  2|      |29  3V3
//     PWM  (D0~)  PC3  3|      |28  PC2 (D 24~)     PWM
//     PWM  (D1~)  PC4  4|      |27  PC1 (D 23~)     PWM
//     SCK  (D 2)  PC5  5|      |26  PE5 (D 22)      LED NSS
//     MOSI (D 3)  PC6  6|      |25  PB0 (D 21,AI 5)
//     MISO (D 4)  PC7  7|      |24  PB1 (D 20,AI 4)
//     PWM  (D 5~) PD0  8|      |23  PB2 (D 19,AI 3)
//     SWIM (D 6)  PD1  9|      |22  PB3 (D 18,AI 2)
//     PWM  (D 7~) PD2 10|      |21  PB4 (D 17,AI 1) SCL
//     PWM  (D 8~) PD3 11|      |20  PB5 (D 16,AI 0) SDA
//     PWM  (D 9~) PD4 12|      |19  PF4 (D 15)
//     TX   (D 10) PD5 13|      |18  PA2 (D 14)
//     RX   (D 11) PD6 14|      |17  PA1 (D 13)
//          (D 12) PD7 15|      |16  NRST             

//                       +-oooo-+

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
	PC,   /* 0 */
	PC,
	PC,
	PC,
	PC,   /* 4 */
	PD,   /* 5 */
	PD,
	PD,
	PD,
	PD,
	PD,
	PD,
	PD,  /* 12 */
	PA,  /* 13 */
	PA,  /* 14 */
	PF,  /* 15 */
	PB,  /* 16 */
	PB,
	PB,
	PB,
	PB,
	PB,  /* 21 */
	PE,  /* 22 */
	PC,  /* 23 */
	PC,  /* 24 */
};

const uint8_t PROGMEM digital_pin_to_bit_mask_PGM[] = {
	_BV(3), /* 0, port C */
	_BV(4),
	_BV(5),
	_BV(6),
	_BV(7),
	_BV(0), /* 5, port D */
	_BV(1),
	_BV(2),
	_BV(3), 
	_BV(4),
	_BV(5),
	_BV(6),
	_BV(7),
	_BV(1), /* 13, port A */
	_BV(2), 
	_BV(4), /* 15, port F */
	_BV(5), /* 16, port B */
	_BV(4),
	_BV(3),
	_BV(2),
	_BV(1),
	_BV(0),
	_BV(5), /* 22, port E */
	_BV(1), /* 23, port C */
	_BV(2),
};

const uint8_t PROGMEM digital_pin_to_timer_PGM[] = {
	TIMER13,	// 0
	TIMER14,	// 1
	NOT_ON_TIMER,	// 2
	NOT_ON_TIMER,	// 3
	NOT_ON_TIMER,	// 4
	TIMER32,	// 5
	NOT_ON_TIMER,	// 6
	TIMER31,	// 7
	TIMER22,	// 8
	TIMER21,	// 9
	NOT_ON_TIMER,	// 10
	NOT_ON_TIMER,	// 11
	NOT_ON_TIMER,	// 12
	NOT_ON_TIMER,	// 13
	NOT_ON_TIMER,	// 14
	NOT_ON_TIMER,	// 15
	NOT_ON_TIMER,	// 16
	NOT_ON_TIMER,	// 17
	NOT_ON_TIMER,	// 18
	NOT_ON_TIMER,	// 19
	NOT_ON_TIMER,	// 20
	NOT_ON_TIMER,	// 21
	NOT_ON_TIMER,	// 22
	TIMER11,	// 23
	TIMER12,	// 24
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
