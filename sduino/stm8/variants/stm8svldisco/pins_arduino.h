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

//STM8S003K3 Pin count
#define NUM_DIGITAL_PINS			28
#define NUM_ANALOG_INPUTS			4
#define NUM_DIGITAL_INPUTS			NUM_DIGITAL_PINS - NUM_ANALOG_INPUTS
//#define analogInputToDigitalPin(p)  ((p < NUM_ANALOG_INPUTS) ? (p) + (NUM_DIGITAL_PINS-NUM_ANALOG_INPUTS) : -1)


/* on the STM8S the logical pin numbers are really confusing instead
 * of beeing helpful. So maybe it is better to use these Portpin-Names
 * instead?
 */
enum portpin {
	PA1, /* 0 */
	PA2,
	PA3,
	PF4,
	PB7,
	PB6,
	PB5,
	PB4,
	PB3,
	PB2,
	PB1,
	PB0,
	PE5,
	PC1,
	PC2,
	PC3,
	PC4,
	PC5,
	PC6,
	PC7,
	PD0,
	PD1,
	PD2,
	PD3,
	PD4,
	PD5,
	PD6,
	PD7,
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


#ifdef SUPPORT_ALTERNATE_MAPPINGS
 // using alternate functions adds 3 more PWM pins, total of 7 PWM pins
 #define digitalPinHasPWM(p)	( (p)==2 | ((p)>=5&(p)<=9) | (p)==12 )
#else
 // standard case: only 4 regular PWM pins
 #define digitalPinHasPWM(p)	( (p)==2 | (p)==5 | (p)==6 | (p)==12 )
#endif

#define PIN_SPI_SS    (PE5)	// 2
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
#define	SS		PIN_SPI_SS
#define	MOSI	PIN_SPI_MOSI
#define	MISO	PIN_SPI_MISO
#define	SCK		PIN_SPI_SCK

#define PIN_WIRE_SDA        (PB5)	// 3
#define PIN_WIRE_SCL        (PB4)	// 4

/* SDCC workaround
static const uint8_t SDA = PIN_WIRE_SDA;
static const uint8_t SCL = PIN_WIRE_SCL;
*/
#define	SDA	PIN_WIRE_SDA
#define	SCL	PIN_WIRE_SCL

#define PIN_LED_BUILTIN (PD0)	// sduino: pin for the buildin LED
#define PIN_TX	(PD5)		// sduino: pin for TX line
#define PIN_RX	(PD6)		// sduino: pin for RX line

#define LED_BUILTIN (PD0)	// pin for the buildin LED
#define BUTTON (PB7)		// pin for the buildin BUTTON, pin 3

//defenition for analog channels on STM8S003K3
#define	A0	0	/**< Analog channel 0 */
#define	A1	1	/**< Analog channel 1 */
#define	A2	2	/**< Analog channel 2 */
#define	A3	3	/**< Analog channel 3 */


// Distinguish between ADC channel number and digital pin number.
// Note that for value 6 both ranges overlap and it is used a pin number.
//
// values 0..5: ADC channel number, no conversion
// values 6..15: digital pin numbers, convert to ADC channel number
//#define analogPinToChannel(P)	( (P)<6 ? (P) : digitalPinToAnalogChannelMap[(P-6)] )
//extern const uint8_t digitalPinToAnalogChannelMap[];

/*FIXME
#define digitalPinToPCICR(p)    (((p) >= 0 && (p) <= 21) ? (&PCICR) : ((uint8_t *)0))
#define digitalPinToPCICRbit(p) (((p) <= 7) ? 2 : (((p) <= 13) ? 0 : 1))
#define digitalPinToPCMSK(p)    (((p) <= 7) ? (&PCMSK2) : (((p) <= 13) ? (&PCMSK0) : (((p) <= 21) ? (&PCMSK1) : ((uint8_t *)0))))
#define digitalPinToPCMSKbit(p) (((p) <= 7) ? (p) : (((p) <= 13) ? ((p) - 8) : ((p) - 14)))

#define digitalPinToInterrupt(p)  ((p) == 2 ? 0 : ((p) == 3 ? 1 : NOT_AN_INTERRUPT))
*/

#ifdef ARDUINO_MAIN

//	Pinout of ST ValueLine Discovery Board MB1008
//	with STM8S003K3					
//						
//  CN1	Pin		Pin		Pin 		Type Main function / Alternate function
//  pin	(chip)	Arduino	name
//  1	1				NRST 		I / O	Reset
//  2	2		0		OSCIN / PA1 	I / O	Port A1
//  3	3		1		OSCOUT / PA2 	I / O	Port A2
//  4	4				GND			S	Digital ground
//  5	4				GND			S	Digital ground
//	6	5				VCAP		S	1.8 V regulator capacitor 
//	NC	6				VDD			S	Digital power supply
//	7	7		2		PA3			I/O	Port A3 Timer 2 - channel 3 / SPI master slave
//	8	8		3		PF4			I/O	Port F4  
//									
//									
//	CN2	Pin		Pin		Pin 		Type Main function / Alternate function
//	pin	(chip)	Arduino	name	
//	1	17		12		PE5			I/O	Port E5 SPI master slave 
//	2	18		13		PC1			I/O	Port C1 Timer 1 - channel 1 / UART2 synchronous clock
//	3	19		14		PC2			I/O	Port C2 Timer 1 - channel 2 
//	4	20		15		PC3			I/O	Port C3 Timer 1 - channel 3 
//	5	21		16		PC4			I/O	Port C4 Timer 1 - channel 4 
//	6	22		17		PC5			I/O	Port C5 SPI clock 
//	7	23		18		PC6			I/O	Port C6 SPI master out / slave in 
//	8	24		19		PC7			I/O	Port C7 SPI master in / slave out
//									
//									
//	CN3	Pin		Pin		Pin 		Type Main function / Alternate function
//	pin	(chip)	Arduino	name	
//	1	9		4 		PB7/B1		IO	Port B7 BUTTON
//	2	10		5 		PB6			IO	Port B6
//	3	11		6 		PB5			I/O	Port B5 I2C data
//	4	12		7 		PB4			I/O	Port B4 I2C clock
//	5	13		8 		PB3			I/O	Port B3 Analog input 3 / Timer 1 external trigger	
//	6	14		9 		PB2			I/O	Port B2 Analog input 2 / Timer 1 inverted channel 3	
//	7	15		10		PB1			I/O	Port B1 Analog input 1 / Timer 1 inverted channel 2	
//	8	16		11		PB0			I/O	Port B0 Analog input 0 / Timer 1 inverted channel 1	
//									
//									
//	CN4	Pin		Pin		Pin 		Type Main function / Alternate function
//	pin	(chip)	Arduino	name	
//	1	25		20		PD0/LD1		I/O Port D0 Timer 1 - break input / conf. clock output LED_BUILTIN
//	2	26		21		PD1/SWIM	I/O Port D1 SWIM data interface 	
//	3	27		22		PD2			I/O	Port D2 Timer 2 - channel 3	
//	4	28		23		PD3			I/O	Port D3 Timer 2 - channel 2 / ADC external trigger	
//	5	29		24		PD4			I/O	Port D4 Timer 2 - channel 1 / BEEP output	
//	6	30		25		PD5			I/O	Port D5 UART1 data transmit 	
//	7	31		26		PD6			I/O	Port D6 UART1 data receive 	
//	8	32		27		PD7			I/O	Port D7 Top level interrupt / Timer 1 - channel 4


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
	PF,
	PB,
	PB,
	PB,
	PB,
	PB,
	PB,
	PB,
	PB,
	PE,
	PC,
	PC,
	PC,
	PC,
	PC,
	PC,
	PC,
	PD,
	PD,
	PD,
	PD,
	PD,
	PD,
	PD,
	PD,
};

const uint8_t PROGMEM digital_pin_to_bit_mask_PGM[] = {
	_BV(1),	//PA1, PA /* 0 */ /* 0 */
	_BV(2),	//PA2,
	_BV(3),	//PA3,
	_BV(4),	//PF4, PF
	_BV(7),	//PB7,
	_BV(6),	//PB6, PB
	_BV(5),	//PB5,
	_BV(4),	//PB4,
	_BV(3),	//PB3,
	_BV(2),	//PB2,
	_BV(1),	//PB1,
	_BV(0),	//PB0,
	_BV(5),	//PE5, PE
	_BV(1),	//PC1, PC
	_BV(2),	//PC2,
	_BV(3),	//PC3,
	_BV(4),	//PC4,
	_BV(5),	//PC5,
	_BV(6),	//PC6,
	_BV(7),	//PC7,
	_BV(0),	//PD0, PD
	_BV(1),	//PD1,
	_BV(2),	//PD2,
	_BV(3),	//PD3,
	_BV(4),	//PD4,
	_BV(5),	//PD5,
	_BV(6),	//PD6,
	_BV(7),	//PD7,
};
/*
*mapps the pins to te right Timers, if possible, otherwise returns NOT_ON_TIMER
*https://garretlab.web.fc2.com/en/arduino/inside/hardware/arduino/avr/cores/arduino/Arduino.h/digitalPinToTimer.html
*/

const uint8_t PROGMEM digital_pin_to_timer_PGM[] = {
	NOT_ON_TIMER,	//0 PA1, PA /* 0 */
	NOT_ON_TIMER,	//1 PA2,
	TIMER23,		//2 PA3,
	NOT_ON_TIMER,	//3 PF4, PF
	NOT_ON_TIMER,	//4 PB7,
	NOT_ON_TIMER,	//5 PB6, PB
	NOT_ON_TIMER,	//6 PB5,
	NOT_ON_TIMER,	//7 PB4,
	NOT_ON_TIMER,	//8 PB3,
	NOT_ON_TIMER,	//9 PB2,
	NOT_ON_TIMER,	//10 PB1,
	NOT_ON_TIMER,	//11 PB0,
	NOT_ON_TIMER,	//12 PE5, PE
	TIMER11,		//13 PC1, PC
	TIMER12,		//14 PC2,
	TIMER13,		//15 PC3,
	TIMER14,		//16 PC4,
	NOT_ON_TIMER,	//17 PC5,
	NOT_ON_TIMER,	//18 PC6,
	NOT_ON_TIMER,	//19 PC7,
	NOT_ON_TIMER,	//20 PD0, PD
	NOT_ON_TIMER,	//21 PD1,
	NOT_ON_TIMER,	//22 PD2,
	TIMER22,		//23 PD3,
	TIMER21,		//24 PD4,
	NOT_ON_TIMER,	//25 PD5,
	NOT_ON_TIMER,	//26 PD6,
	NOT_ON_TIMER,	//27 PD7,
};


#endif /* ARDUINO_MAIN */

#define NEED_TIMER_23
#define NEED_TIMER_11_12

#ifdef SUPPORT_ALTERNATE_MAPPINGS
 #define NEED_TIMER_11_12
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


