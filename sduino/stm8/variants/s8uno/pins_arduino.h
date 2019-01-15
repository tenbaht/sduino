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


#define NUM_DIGITAL_PINS            23
#define NUM_ANALOG_INPUTS           9  // skipping A6 and A7, AREF is A8
#define analogInputToDigitalPin(p)  ((p < 9) ? (p) + 14 : -1)


/* functional pin mapping for the STM8S105 used on the sduino-uno board.
 *
 * This pin mapping tries to reassemble the original Arduino Uno mapping
 * for the ATmega328 as closely as possible.
 */
enum portpin {
	PD6,	/* IO0/RX	PD6/UART2_RX		*/
	PD5,	/* IO1/TX 	PD5/UART2_TX		*/
	PD7,	/* IO2 		PD7/TLI [TIM1_CH4]	*/
	PD2,	/* IO3~		PD2 (HS)/TIM3_CH1 [TIM2_CH3] */
	PD0,	/* IO4~		PD0 (HS)/TIM3_CH2 [TIM1_BKIN]/[CLK_COO] */
	PD4,	/* IO5~		PD4 (HS)/TIM2_CH1 [BEEP] */
	PD3,	/* IO6~		PD3 (HS)/TIM2_CH2/ADC_ETR */
	PD1,	/* IO7 		PD1 (HS)/SWIM		*/
	PC1,	/* IO8~		PC1 (HS)/TIM1_CH1/UART2_CK */
	PC3,	/* IO9~		PC3 (HS)/TIM1_CH3	*/
	PC4,	/* IO10~ 	PC4 (HS)/TIM1_CH4	*/
	PC6,	/* IO11/MOSI 	PC6 (HS)/SPI_MOSI	*/
	PC7,	/* IO12/MISO 	PC7 (HS)/SPI_MISO	*/
	PC5,	/* IO13/SCK 	PC5 (HS)/SPI_SCK	*/
	PB0,	/* AD0 		PB0/AIN0 [TIM1_CH1N]	*/
	PB1,	/* AD1 		PB1/AIN1 [TIM1_CH2N]	*/
	PB2,	/* AD2 		PB2/AIN2 [TIM1_CH3N]	*/
	PB3,	/* AD3 		PB3/AIN3 [TIM1_ETR]	*/
	PB5,	/* AD4/SDA 	PB5/AIN5 [I2C_SDA]	*/
	PB4,	/* AD5/SCL 	PB4/AIN4 [I2C_SCL]	*/
	PF4=22,	/* AREF		PF4/AIN12		*/
	PC2,	/* IO23~ 	PC2 (HS)/TIM1_CH2	*/
	PE5,	/* IO24/SS 	PE5/SPI_NSS		*/
};



// PWM on pins 3-6, 8-10, 14
 #define digitalPinHasPWM(p)	( ( (p)>=3 & (p)<=10 & (p)!=7 ) \
 				| (p)==14 \
 )

// TODO: find a way to use pin 10 (PC4) as software controlled SS
#define PIN_SPI_SS    (PE5)	// 24
#define PIN_SPI_MOSI  (PC6)	// 11
#define PIN_SPI_MISO  (PC7)	// 12
#define PIN_SPI_SCK   (PC5)	// 13

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

#define PIN_WIRE_SDA        (PB5)	// 20
#define PIN_WIRE_SCL        (PB4)	// 21

/* SDCC workaround
static const uint8_t SDA = PIN_WIRE_SDA;
static const uint8_t SCL = PIN_WIRE_SCL;
*/
#define	SDA	PIN_WIRE_SDA
#define	SCL	PIN_WIRE_SCL

#define PIN_LED_BUILTIN (PC5)	// sduino: pin for the buildin LED, pin 13
#define PIN_TX	(PD5)		// sduino: pin for TX line, pin 1
#define PIN_RX	(PD6)		// sduino: pin for RX line, pin 0

#define LED_BUILTIN (PC5)	// pin for the buildin LED, pin 13

#define PIN_A0   (PB0)		// 14, Ain0
#define PIN_A1   (PB1)		// 15, Ain1
#define PIN_A2   (PB2)		// 16, Ain2
#define PIN_A3   (PB3)		// 17, Ain3
#define PIN_A4   (PB5)		// 18, Ain5
#define PIN_A5   (PB4)		// 19, Ain4

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

//#define NO_ANALOG	0xff

// map the logical pin numbers to the physical ADC channels:  FIXME: produces rubbish for pins 20 and 21
// pin 22     -> channel 12
// pin 18,19  -> channel 5,4 (reverse order!)
// pin 14..17 -> channel 0-3
// smaller numbers are not modified but used as channel numbers directly.
#define analogPinToChannel(P) ( (P)==22 ? 12 : ( \
				(P)>=18 ? 23-(P) : ( \
				(P)>=14 ? (P)-14 : \
					  (P) \
				)))

/* alternative way to define the mapping:
#define analogPinToChannel(P) ( (P)<14 ? (P) : ( \
				(P)<18 ? (P)-14 : ( \
				(p)!=22 ? 23-(P) : \
					  12 \
				)))
*/


/*FIXME
#define digitalPinToPCICR(p)    (((p) >= 0 && (p) <= 21) ? (&PCICR) : ((uint8_t *)0))
#define digitalPinToPCICRbit(p) (((p) <= 7) ? 2 : (((p) <= 13) ? 0 : 1))
#define digitalPinToPCMSK(p)    (((p) <= 7) ? (&PCMSK2) : (((p) <= 13) ? (&PCMSK0) : (((p) <= 21) ? (&PCMSK1) : ((uint8_t *)0))))
#define digitalPinToPCMSKbit(p) (((p) <= 7) ? (p) : (((p) <= 13) ? ((p) - 8) : ((p) - 14)))

#define digitalPinToInterrupt(p)  ((p) == 2 ? 0 : ((p) == 3 ? 1 : NOT_AN_INTERRUPT))
*/

#ifdef ARDUINO_MAIN

// STMicroelectronics STM8S105K6T, -K6U
//
//                    pwm         PWM PWM PWM     PWM
//                    D2  D0  D1  D5  D6  D3  D7  D4
//                    PD7 PD6 PD5 PD4 PD3 PD2 PD1 PD0
//                    32  31  30  29  28  27  26  25
//                  +--------------------------------+
//          NRST  1 | o                              | 24  PC7  D12
//                  |                                |
//           PA1  2 |                                | 23  PC6  D11
//                  |                                |
//           PA2  3 |                                | 22  PC5  D13
//                  |                                |
//           VSS  4 |                                | 21  PC4  D10  PWM
//                  |                                |
//          VCAP  5 |                                | 20  PC3  D9   PWM
//                  |                                |
//           VDD  6 |                                | 19  PC2  D23  PWM
//                  |                                |
//         VDDIO  7 |                                | 18  PC1  D8   PWM
//                  |                                |
// AREF/A8   PF4  8 |                                | 17  PE5  D24
//                  +--------------------------------+
//                     9  10  11  12  13  14  15  16
//                  VDDA VSSA PB5 PB4 PB3 PB2 PB1 PB0
//                            A4  A5  A3  A2  A1  A0
//                                        pwm pwm pwm
//
// (PWM: always available as PWM)
// (pwm: PWM as alternate function -- en-/disabled for all such pins at once)

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
	PD, /* 0 */
	PD,
	PD,
	PD,
	PD,
	PD,
	PD,
	PD,
	PC, /* 8 */
	PC,
	PC,
	PC,
	PC,
	PC,
	PB, /* 14 */
	PB,
	PB,
	PB,
	PB,
	PB,
	PB,
	PB,
	PF, /* 22 */
	PC, /* 23 */
	PE, /* 24 */
};

const uint8_t PROGMEM digital_pin_to_bit_mask_PGM[] = {
	_BV(6), /* 0, port D */
	_BV(5),
	_BV(7),
	_BV(2),
	_BV(0),
	_BV(4),
	_BV(3),
	_BV(1),
	_BV(1), /* 8, port C */
	_BV(3),
	_BV(4),
	_BV(6),
	_BV(7),
	_BV(5),
	_BV(0), /* 14, port B */
	_BV(1),
	_BV(2),
	_BV(3),
	_BV(5),
	_BV(4),
	_BV(6),
	_BV(7),
	_BV(4), /* 22, port F */
	_BV(2), /* 23, port C */
	_BV(5), /* 24, port E */
};

const uint8_t PROGMEM digital_pin_to_timer_PGM[] = {
	NOT_ON_TIMER,
	NOT_ON_TIMER,
	NOT_ON_TIMER,
	TIMER31,	// 3
	TIMER32,	// 4
	TIMER21,	// 5
	TIMER22,	// 6
	NOT_ON_TIMER,
	TIMER11,	// 8
	TIMER13,	// 9
	TIMER14,	// 10
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
	NOT_ON_TIMER,
	TIMER12,	// 23
	NOT_ON_TIMER,
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
