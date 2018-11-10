/*
  pins_arduino.h - Pin definition functions for Arduino
  Part of Arduino - http://www.arduino.cc/

  Copyright (c) 2007 David A. Mellis
                2017 Michael Mayer, Benedikt Freisen
                2018 Benedikt Freisen

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


#define NUM_DIGITAL_PINS            70
#define NUM_ANALOG_INPUTS           16
#define analogInputToDigitalPin(p)  ((p < 16) ? (p) + 54 : -1)


/* functional pin mapping for the STM8S208MB used on the sduino MB board.
 *
 * This pin mapping tries to resemble the original Arduino Mega mapping
 * for the ATmega2560 as closely as possible.
 */
enum portpin {
	PA4,
	PA5,
	PH5,
	PH6,
	PA3,
	PH7,
	PD0,
	PD2,
	PD3,
	PD4,
	PC1,
	PC2,
	PC3,
	PC4,
	PD5,
	PD6,
	PG0,
	PG1,
	PE2 = 20,
	PE1,
	PI0,
	PI1,
	PI2,
	PI3,
	PI4,
	PI5,
	PI6,
	PI7,
	PG7,
	PG6,
	PG5,
	PG4,
	PG3,
	PG2,
	PD1,
	PD7,
	PC0,
	PE4,
	PE3,
	PE0,
	PA6,
	PH4,
	PH3,
	PH2,
	PH1,
	PH0,
	PC7 = 50,
	PC6,
	PC5,
	PE5,
	PB0,
	PB1,
	PB2,
	PB3,
	PB5,
	PB4,
	PB6,
	PB7,
	PE7,
	PE6,
	PF0,
	PF3,
	PF4,
	PF5,
	PF6,
	PF7,
};



// PWM on pins 2-13
#define digitalPinHasPWM(p)	( (p)>=2 & (p)<=13 )

#define PIN_SPI_SS    (PE5)	// 53
#define PIN_SPI_MOSI  (PC6)	// 51
#define PIN_SPI_MISO  (PC7)	// 50
#define PIN_SPI_SCK   (PC5)	// 52

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

#define PIN_WIRE_SDA        (PE2)	// 20
#define PIN_WIRE_SCL        (PE1)	// 21

/* SDCC workaround
static const uint8_t SDA = PIN_WIRE_SDA;
static const uint8_t SCL = PIN_WIRE_SCL;
*/
#define	SDA	PIN_WIRE_SDA
#define	SCL	PIN_WIRE_SCL

#define PIN_LED_BUILTIN (PC4)	// sduino: pin for the buildin LED, pin 13
#define PIN_TX	(PA5)		// sduino: pin for TX line, pin 1
#define PIN_RX	(PA4)		// sduino: pin for RX line, pin 0

#define LED_BUILTIN (PC4)	// pin for the buildin LED, pin 13

#define PIN_A0   (PB0)		// 54, Ain0
#define PIN_A1   (PB1)		// 55, Ain1
#define PIN_A2   (PB2)		// 56, Ain2
#define PIN_A3   (PB3)		// 57, Ain3
#define PIN_A4   (PB5)		// 58, Ain5
#define PIN_A5   (PB4)		// 59, Ain4
#define PIN_A6   (PB6)		// 60, Ain6
#define PIN_A7   (PB7)		// 61, Ain7
#define PIN_A8   (PE7)		// 62, Ain8
#define PIN_A9   (PE6)		// 63, Ain9
#define PIN_A10  (PF0)		// 64, Ain10
#define PIN_A11  (PF3)		// 65, Ain11
#define PIN_A12  (PF4)		// 66, Ain12
#define PIN_A13  (PF5)		// 67, Ain13
#define PIN_A14  (PF6)		// 68, Ain14
#define PIN_A15  (PF7)		// 69, Ain15

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
#define	A10	PIN_A10
#define	A11	PIN_A11
#define	A12	PIN_A12
#define	A13	PIN_A13
#define	A14	PIN_A14
#define	A15	PIN_A15

//#define NO_ANALOG	0xff

// map the logical pin numbers to the physical ADC channels:
// pin 60..69 -> channel 6-15
// pin 58,59  -> channel 5,4 (reverse order!)
// pin 54..57 -> channel 0-3
// smaller numbers are not modified but used as channel numbers directly.
#define analogPinToChannel(P) ( (P)>=60 ? (P)-54 : ( \
				(P)>=58 ? 63-(P) : ( \
				(P)>=54 ? (P)-54 : \
					  (P) \
				)))


/*FIXME
#define digitalPinToPCICR(p)    (((p) >= 0 && (p) <= 21) ? (&PCICR) : ((uint8_t *)0))
#define digitalPinToPCICRbit(p) (((p) <= 7) ? 2 : (((p) <= 13) ? 0 : 1))
#define digitalPinToPCMSK(p)    (((p) <= 7) ? (&PCMSK2) : (((p) <= 13) ? (&PCMSK0) : (((p) <= 21) ? (&PCMSK1) : ((uint8_t *)0))))
#define digitalPinToPCMSKbit(p) (((p) <= 7) ? (p) : (((p) <= 13) ? ((p) - 8) : ((p) - 14)))

#define digitalPinToInterrupt(p)  ((p) == 2 ? 0 : ((p) == 3 ? 1 : NOT_AN_INTERRUPT))
*/

#ifdef ARDUINO_MAIN

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
	GPIOG_BaseAddress+2,
	GPIOH_BaseAddress+2,
	GPIOI_BaseAddress+2,
};

const uint16_t PROGMEM port_to_output_PGM[] = {
	NOT_A_PORT,
	GPIOA_BaseAddress,
	GPIOB_BaseAddress,
	GPIOC_BaseAddress,
	GPIOD_BaseAddress,
	GPIOE_BaseAddress,
	GPIOF_BaseAddress,
	GPIOG_BaseAddress,
	GPIOH_BaseAddress,
	GPIOI_BaseAddress,
};

const uint16_t PROGMEM port_to_input_PGM[] = {
	NOT_A_PORT,
	GPIOA_BaseAddress+1,
	GPIOB_BaseAddress+1,
	GPIOC_BaseAddress+1,
	GPIOD_BaseAddress+1,
	GPIOE_BaseAddress+1,
	GPIOF_BaseAddress+1,
	GPIOG_BaseAddress+1,
	GPIOH_BaseAddress+1,
	GPIOI_BaseAddress+1,
};

const uint8_t PROGMEM digital_pin_to_port_PGM[] = {
	PA, /* 0 */
	PA,
	PH, /* 2 */
	PH,
	PA, /* 4 */
	PH, /* 5 */
	PD, /* 6 */
	PD,
	PD,
	PD,
	PC, /* 10 */
	PC,
	PC,
	PC,
	PD, /* 14 */
	PD,
	PG, /* 16 */
	PG,
	PG, /* DUMMY */
	PG, /* DUMMY */
	PE, /* 20 */
	PE,
	PI, /* 22 */
	PI,
	PI,
	PI,
	PI,
	PI,
	PI,
	PI,
	PG, /* 30 */
	PG,
	PG,
	PG,
	PG,
	PG,
	PD, /* 36 */
	PD,
	PC, /* 38 */
	PE, /* 39 */
	PE,
	PE,
	PA, /* 42 */
	PH, /* 43 */
	PH,
	PH,
	PH,
	PH,
	PH, /* DUMMY */
	PH, /* DUMMY */
	PC, /* 50 */
	PC,
	PC,
	PE, /* 53 */
	PB, /* 54 */
	PB,
	PB,
	PB,
	PB,
	PB,
	PB,
	PB,
	PE, /* 62 */
	PE,
	PF, /* 64 */
	PF,
	PF,
	PF,
	PF,
	PF,
};

const uint8_t PROGMEM digital_pin_to_bit_mask_PGM[] = {
	_BV(4), /* 0, port A */
	_BV(5),
	_BV(5), /* 2, port H */
	_BV(6),
	_BV(3), /* 4, port A */
	_BV(7), /* 5, port H */
	_BV(0), /* 6, port D */
	_BV(2),
	_BV(3),
	_BV(4),
	_BV(1), /* 10, port C */
	_BV(2),
	_BV(3),
	_BV(4),
	_BV(5), /* 14, port D */
	_BV(6),
	_BV(0), /* 16, port G */
	_BV(1),
	_BV(0), /* DUMMY */
	_BV(0), /* DUMMY */
	_BV(2), /* 20, port E */
	_BV(1),
	_BV(0), /* 22, port I */
	_BV(1),
	_BV(2),
	_BV(3),
	_BV(4),
	_BV(5),
	_BV(6),
	_BV(7),
	_BV(7), /* 30, port G */
	_BV(6),
	_BV(5),
	_BV(4),
	_BV(3),
	_BV(2),
	_BV(1), /* 36, port D */
	_BV(7),
	_BV(0), /* 38, port C */
	_BV(4), /* 39, port E */
	_BV(3),
	_BV(0),
	_BV(6), /* 42, port A */
	_BV(4), /* 43, port H */
	_BV(3),
	_BV(2),
	_BV(1),
	_BV(0),
	_BV(0), /* DUMMY */
	_BV(0), /* DUMMY */
	_BV(7), /* 50, port C */
	_BV(6),
	_BV(5),
	_BV(5), /* 53, port E */
	_BV(0), /* 54, port B */
	_BV(1),
	_BV(2),
	_BV(3),
	_BV(5),
	_BV(4),
	_BV(6),
	_BV(7),
	_BV(7), /* 62, port E */
	_BV(6),
	_BV(0), /* 64, port F */
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
	NOT_ON_TIMER,
	NOT_ON_TIMER,//TIMER23,
	NOT_ON_TIMER,
	TIMER32,
	TIMER31,
	TIMER22,
	TIMER21,
	TIMER11,
	TIMER12,
	TIMER13,
	TIMER14,
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
