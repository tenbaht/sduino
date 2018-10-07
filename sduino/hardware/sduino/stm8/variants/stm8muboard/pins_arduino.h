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

#define NUM_DIGITAL_PINS            37
#define NUM_ANALOG_INPUTS           16
//#define analogInputToDigitalPin(p)  ((p < NUM_ANALOG_INPUTS) ? (p) + (NUM_DIGITAL_PINS-NUM_ANALOG_INPUTS) : -1)


enum digital_pin {
	AUTO_MODE,     /* 0  = PD7 = AUTO_MODE */
	IO_1,          /* 1  = PE5 = IO_1 */
	IO_2,          /* 2  = PD2 = IO_2 */
	IO_3,          /* 3  = PD0 = IO_3 */
	IO_4,          /* 4  = PA6 = IO_4 */
	IO_5,          /* 5  = PH0 = IO_5 */
	IO_6,          /* 6  = PH1 = IO_6 */
	IO_7,          /* 7  = PF6 = IO_7 = AIN14 */
	IO_8,          /* 8  = PF5 = IO_8 = AIN13 */
	IO_9,          /* 9  = PF4 = IO_9 = AIN12 */
	IO_10,         /* 10 = PF3 = IO_10 = AIN11 */
	IO_11,         /* 11 = PF0 = IO_11 = AIN10 */
	IO_12,         /* 12 = PB7 = IO_12 = AIN7 */
	IO_13,         /* 13 = PB6 = IO_13 = AIN6 */
	IO_14,         /* 14 = PB1 = IO_14 = AIN1 */
	IO_15,         /* 15 = PB0 = IO_15 = AIN0 */
	IO_16,         /* 16 = PH4 = IO_16 */
	TIM_1,         /* 17 = PC1 = TIM_1 = TIM1_CC1 */
	TIM_2,         /* 18 = PD3 = TIM_2 = TIM2_CC2 */
	LIN_RX,        /* 19 = PD6 = LIN_RX */
	LIN_TX,        /* 20 = PD5 = LIN_TX */
	LED_GREEN,     /* 21 = PH2 = LED_GREEN */
	LED_RED,       /* 22 = PH3 = LED_RED */
	LS_1,          /* 23 = PE6 = LS_1 */
	LS_2,          /* 24 = PA3 = LS_2 = TIM2_CC3 */
	LS_3,          /* 25 = PC0 = LS_3 */
	BRIDGE_1,      /* 26 = PH5 = BRIDGE_1 */
	BRIDGE_2,      /* 27 = PH6 = BRIDGE_2 */
	BRIDGE_3,      /* 28 = PH7 = BRIDGE_3 */
	BRIDGE_4,      /* 29 = PE7 = BRIDGE_4 */
	SPI_MOSI,      /* 30 = PC6 = SPI_MOSI */
	SPI_MISO,      /* 31 = PC7 = SPI_MISO */
	SPI_SCK,       /* 32 = PC5 = SPI_SCK */
        I2C_SDA,       /* 33 = PE2 = I2C_SDA */
        I2C_SCL,       /* 34 = PE1 = I2C_SCL */
	UART_TX,       /* 35 = PD5 = UART_TX */
        UART_RX,       /* 36 = PD6 = UART_RX */
};


enum analog_pin {
	A15,    /* AIN0 = PB0 = io_15 */
	A14,    /* AIN1 = PB1 = io_14 */
	VD_LV,  /* AIN2 = PB3 = VOLT_DIV_LV */
	VD_HV,  /* AIN3 = PB2 = VOLT_DIV_HV */
	NTC,    /* AIN4 = PB4 = TEMP_NTC */
	POTI,   /* AIN5 = PB5 = POTI */
	A13,    /* AIN6 = PB6 = io_13 */
	A12,    /* AIN7 = PB7 = io_12 */
	_XXX1,  /* not available on muBoard */
	_XXX2,  /* not available on muBoard */
	A11,    /* AIN10 = PF0 = io_11 */
	A10,    /* AIN11 = PF3 = io_10 */
	A9,     /* AIN12 = PF4 = io_9 */
	A8,     /* AIN13 = PF5 = io_8 */
	A7,     /* AIN14 = PF6 = io_7 */
	VCORE , /* AIN15 = PF7 = VOLT_CORE */
};


// PWM on pins TIM1, TIM2 and LOW_SWITCH_2
#define digitalPinHasPWM(p)	( (p)==17 | (p)==18(p) | (p)==24 )

#define PIN_SPI_SS	(IO_1)		// PE5
#define PIN_SPI_MOSI	(SPI_MOSI)	// PC6
#define PIN_SPI_MISO	(SPI_MISO)	// PC7
#define PIN_SPI_SCK	(SPI_SCK)	// PC5
#define SS		PIN_SPI_SS
#define	MOSI		PIN_SPI_MOSI
#define	MISO		PIN_SPI_MISO
#define	SCK		PIN_SPI_SCK


#define PIN_WIRE_SDA	(I2C_SDA)	// PE2
#define PIN_WIRE_SCL	(I2C_SCL)	// PE1
#define	SDA		PIN_WIRE_SDA
#define	SCL		PIN_WIRE_SCL

// by default use red LED
#define LED_BUILTIN   (LED_RED)
#define BUTTON	      (IO_1)		// PE5

#define PIN_TX		(UART_TX)	// PD5
#define PIN_RX		(UART_RX)	// PD6

//#define NO_ANALOG	0xff

// map the logical pin numbers to the physical ADC channels via above enum:
#define analogPinToChannel(P) ( P )


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
#ifdef GPIOA
	GPIOA_BaseAddress+2,
#endif
#ifdef GPIOB
	GPIOB_BaseAddress+2,
#endif
#ifdef GPIOC
	GPIOC_BaseAddress+2,
#endif
#ifdef GPIOD
	GPIOD_BaseAddress+2,
#endif
#ifdef GPIOE
	GPIOE_BaseAddress+2,
#endif
#ifdef GPIOF
	GPIOF_BaseAddress+2,
#endif
#ifdef GPIOG
	GPIOG_BaseAddress+2,
#endif
#ifdef GPIOH
	GPIOH_BaseAddress+2,
#endif
};

const uint16_t PROGMEM port_to_output_PGM[] = {
	NOT_A_PORT,
#ifdef GPIOA
	GPIOA_BaseAddress,
#endif
#ifdef GPIOB
	GPIOB_BaseAddress,
#endif
#ifdef GPIOC
	GPIOC_BaseAddress,
#endif
#ifdef GPIOD
	GPIOD_BaseAddress,
#endif
#ifdef GPIOE
	GPIOE_BaseAddress,
#endif
#ifdef GPIOF
	GPIOF_BaseAddress,
#endif
#ifdef GPIOG
	GPIOG_BaseAddress,
#endif
#ifdef GPIOH
	GPIOH_BaseAddress,
#endif
#ifdef GPIOI
	GPIOI_BaseAddress,
#endif
};

const uint16_t PROGMEM port_to_input_PGM[] = {
	NOT_A_PORT,
#ifdef GPIOA
	GPIOA_BaseAddress+1,
#endif
#ifdef GPIOB
	GPIOB_BaseAddress+1,
#endif
#ifdef GPIOC
	GPIOC_BaseAddress+1,
#endif
#ifdef GPIOD
	GPIOD_BaseAddress+1,
#endif
#ifdef GPIOE
	GPIOE_BaseAddress+1,
#endif
#ifdef GPIOF
	GPIOF_BaseAddress+1,
#endif
#ifdef GPIOG
	GPIOG_BaseAddress+1,
#endif
#ifdef GPIOH
	GPIOH_BaseAddress+1,
#endif
};

const uint8_t PROGMEM digital_pin_to_port_PGM[] = {
	PD,          /* 0  = PD7 = AUTO_MODE */
	PE,          /* 1  = PE5 = IO_1 */
	PD,          /* 2  = PD2 = IO_2 */
	PD,          /* 3  = PD0 = IO_3 */
	PA,          /* 4  = PA6 = IO_4 */
	PH,          /* 5  = PH0 = IO_5 */
	PH,          /* 6  = PH1 = IO_6 */
	PF,          /* 7  = PF6 = IO_7 = AIN14 */
	PF,          /* 8  = PF5 = IO_8 = AIN13 */
	PF,          /* 9  = PF4 = IO_9 = AIN12 */
	PF,          /* 10 = PF3 = IO_10 = AIN11 */
	PF,          /* 11 = PF0 = IO_11 = AIN10 */
	PB,          /* 12 = PB7 = IO_12 = AIN7 */
	PB,          /* 13 = PB6 = IO_13 = AIN6 */
	PB,          /* 14 = PB1 = IO_14 = AIN1 */
	PB,          /* 15 = PB0 = IO_15 = AIN0 */
	PH,          /* 16 = PH4 = IO_16 */
	PC,          /* 17 = PC1 = TIM_1 = TIM1_CC1 */
	PD,          /* 18 = PD3 = TIM_2 = TIM2_CC2 */
	PD,          /* 19 = PD6 = LIN_RX */
	PD,          /* 20 = PD5 = LIN_TX */
	PH,          /* 21 = PH2 = LED_GREEN */
	PH,          /* 22 = PH3 = LED_RED */
	PE,          /* 23 = PE6 = LS_1 */
	PA,          /* 24 = PA3 = LS_2 = TIM2_CC3 */
	PC,          /* 25 = PC0 = LS_3 */
	PH,          /* 26 = PH5 = BRIDGE_1 */
	PH,          /* 27 = PH6 = BRIDGE_2 */
	PH,          /* 28 = PH7 = BRIDGE_3 */
	PE,          /* 29 = PE7 = BRIDGE_4 */
	PC,          /* 30 = PC6 = SPI_MOSI */
	PC,          /* 31 = PC7 = SPI_MISO */
	PC,          /* 32 = PC5 = SPI_SCK */
        PE,          /* 33 = PE2 = I2C_SDA */
        PE,          /* 34 = PE1 = I2C_SCL */
	PD,          /* 35 = PD5 = UART_TX */
        PD,          /* 36 = PD6 = UART_RX */
};

const uint8_t PROGMEM digital_pin_to_bit_mask_PGM[] = {
	_BV(7),      /* 0  = PD7 = AUTO_MODE */
	_BV(5),      /* 1  = PE5 = IO_1 */
	_BV(2),      /* 2  = PD2 = IO_2 */
	_BV(0),      /* 3  = PD0 = IO_3 */
	_BV(6),      /* 4  = PA6 = IO_4 */
	_BV(0),      /* 5  = PH0 = IO_5 */
	_BV(1),      /* 6  = PH1 = IO_6 */
	_BV(6),      /* 7  = PF6 = IO_7 = AIN14 */
	_BV(5),      /* 8  = PF5 = IO_8 = AIN13 */
	_BV(4),      /* 9  = PF4 = IO_9 = AIN12 */
	_BV(3),      /* 10 = PF3 = IO_10 = AIN11 */
	_BV(0),      /* 11 = PF0 = IO_11 = AIN10 */
	_BV(7),      /* 12 = PB7 = IO_12 = AIN7 */
	_BV(6),      /* 13 = PB6 = IO_13 = AIN6 */
	_BV(1),      /* 14 = PB1 = IO_14 = AIN1 */
	_BV(0),      /* 15 = PB0 = IO_15 = AIN0 */
	_BV(4),      /* 16 = PH4 = IO_16 */
	_BV(1),      /* 17 = PC1 = TIM_1 = TIM1_CC1 */
	_BV(3),      /* 18 = PD3 = TIM_2 = TIM2_CC2 */
	_BV(6),      /* 19 = PD6 = LIN_RX */
	_BV(5),      /* 20 = PD5 = LIN_TX */
	_BV(2),      /* 21 = PH2 = LED_GREEN */
	_BV(3),      /* 22 = PH3 = LED_RED */
	_BV(6),      /* 23 = PE6 = LS_1 */
	_BV(3),      /* 24 = PA3 = LS_2 = TIM2_CC3 */
	_BV(0),      /* 25 = PC0 = LS_3 */
	_BV(5),      /* 26 = PH5 = BRIDGE_1 */
	_BV(6),      /* 27 = PH6 = BRIDGE_2 */
	_BV(7),      /* 28 = PH7 = BRIDGE_3 */
	_BV(7),      /* 29 = PE7 = BRIDGE_4 */
	_BV(6),      /* 30 = PC6 = SPI_MOSI */
	_BV(7),      /* 31 = PC7 = SPI_MISO */
	_BV(5),      /* 32 = PC5 = SPI_SCK */
        _BV(2),      /* 33 = PE2 = I2C_SDA */
        _BV(1),      /* 34 = PE1 = I2C_SCL */
	_BV(5),      /* 35 = PD5 = UART_TX */
        _BV(6),      /* 36 = PD6 = UART_RX */
};

const uint8_t PROGMEM digital_pin_to_timer_PGM[] = {
	NOT_ON_TIMER,  /* 0  = PD7 = AUTO_MODE */
	NOT_ON_TIMER,  /* 1  = PE5 = IO_1 */
	NOT_ON_TIMER,  /* 2  = PD2 = IO_2 */
	NOT_ON_TIMER,  /* 3  = PD0 = IO_3 */
	NOT_ON_TIMER,  /* 4  = PA6 = IO_4 */
	NOT_ON_TIMER,  /* 5  = PH0 = IO_5 */
	NOT_ON_TIMER,  /* 6  = PH1 = IO_6 */
	NOT_ON_TIMER,  /* 7  = PF6 = IO_7 = AIN14 */
	NOT_ON_TIMER,  /* 8  = PF5 = IO_8 = AIN13 */
	NOT_ON_TIMER,  /* 9  = PF4 = IO_9 = AIN12 */
	NOT_ON_TIMER,  /* 10 = PF3 = IO_10 = AIN11 */
	NOT_ON_TIMER,  /* 11 = PF0 = IO_11 = AIN10 */
	NOT_ON_TIMER,  /* 12 = PB7 = IO_12 = AIN7 */
	NOT_ON_TIMER,  /* 13 = PB6 = IO_13 = AIN6 */
	NOT_ON_TIMER,  /* 14 = PB1 = IO_14 = AIN1 */
	NOT_ON_TIMER,  /* 15 = PB0 = IO_15 = AIN0 */
	NOT_ON_TIMER,  /* 16 = PH4 = IO_16 */
	TIMER11,       /* 17 = PC1 = TIM_1 = TIM1_CC1 */
	TIMER22,       /* 18 = PD3 = TIM_2 = TIM2_CC2 */
	NOT_ON_TIMER,  /* 19 = PD6 = LIN_RX */
	NOT_ON_TIMER,  /* 20 = PD5 = LIN_TX */
	NOT_ON_TIMER,  /* 21 = PH2 = LED_GREEN */
	NOT_ON_TIMER,  /* 22 = PH3 = LED_RED */
	NOT_ON_TIMER,  /* 23 = PE6 = LS_1 */
	NOT_ON_TIMER,  /* TIMER23 24 = PA3 = LS_2 = TIM2_CC3 */
	NOT_ON_TIMER,  /* 25 = PC0 = LS_3 */
	NOT_ON_TIMER,  /* 26 = PH5 = BRIDGE_1 */
	NOT_ON_TIMER,  /* 27 = PH6 = BRIDGE_2 */
	NOT_ON_TIMER,  /* 28 = PH7 = BRIDGE_3 */
	NOT_ON_TIMER,  /* 29 = PE7 = BRIDGE_4 */
	NOT_ON_TIMER,  /* 30 = PC6 = SPI_MOSI */
	NOT_ON_TIMER,  /* 31 = PC7 = SPI_MISO */
	NOT_ON_TIMER,  /* 32 = PC5 = SPI_SCK */
        NOT_ON_TIMER,  /* 33 = PE2 = I2C_SDA */
        NOT_ON_TIMER,  /* 34 = PE1 = I2C_SCL */
	NOT_ON_TIMER,  /* 35 = PD5 = UART_TX */
        NOT_ON_TIMER,  /* 36 = PD6 = UART_RX */
};

#endif

#define NEED_TIMER_11_12
#define NEED_TIMER_21_22
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
