/*
  Arduino.h - Main include file for the Arduino SDK
  Copyright (c) 2005-2013 Arduino Team.  All right reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#ifndef Arduino_h
#define Arduino_h

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include <stdint.h>

//#include <avr/pgmspace.h>
//#include <avr/io.h>
//#include <avr/interrupt.h>
#include <stm8s.h>

#include "binary.h"

// FIXME: workarounds for missing features or unimplemented functions
// cancel out the PROGMEM attribute - used only for atmel CPUs
#define PROGMEM
void yield(void);

// we use pre-defined IRQ function the way wiring does
#define WIRING



#define HIGH 0x1
#define LOW  0x0

#define INPUT 0x0
#define OUTPUT 0x1
#define INPUT_PULLUP 0x2
#define	OUTPUT_OD 0x03
#define OUTPUT_FAST 0x05
#define OUTPUT_OD_FAST 0x07

// undefine mathlib's pi if encountered
#ifdef PI
#undef PI
#endif
#ifdef HALF_PI
#undef HALF_PI
#endif
#ifdef TWO_PI
#undef TWO_PI
#endif

#define PI 3.1415926535897932384626433832795
#define HALF_PI 1.5707963267948966192313216916398
#define TWO_PI 6.283185307179586476925286766559
#define DEG_TO_RAD 0.017453292519943295769236907684886
#define RAD_TO_DEG 57.295779513082320876798154814105
#define EULER 2.718281828459045235360287471352

#define SERIAL  0x0
#define DISPLAY 0x1

#define LSBFIRST 0
#define MSBFIRST 1

#define CHANGE 1
#define FALLING 2
#define RISING 3

/*
#if defined(__AVR_ATtiny24__) || defined(__AVR_ATtiny44__) || defined(__AVR_ATtiny84__)
  #define DEFAULT 0
  #define EXTERNAL 1
  #define INTERNAL1V1 2
  #define INTERNAL INTERNAL1V1
#elif defined(__AVR_ATtiny25__) || defined(__AVR_ATtiny45__) || defined(__AVR_ATtiny85__)
  #define DEFAULT 0
  #define EXTERNAL 4
  #define INTERNAL1V1 8
  #define INTERNAL INTERNAL1V1
  #define INTERNAL2V56 9
  #define INTERNAL2V56_EXTCAP 13
#else  
#if defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__) || defined(__AVR_ATmega1284__) || defined(__AVR_ATmega1284P__) || defined(__AVR_ATmega644__) || defined(__AVR_ATmega644A__) || defined(__AVR_ATmega644P__) || defined(__AVR_ATmega644PA__)
#define INTERNAL1V1 2
#define INTERNAL2V56 3
#else
#define INTERNAL 3
#endif
#define DEFAULT 1
#define EXTERNAL 0
#endif
*/

// undefine stdlib's abs if encountered
#ifdef abs
#undef abs
#endif

#define min(a,b) ((a)<(b)?(a):(b))
#define max(a,b) ((a)>(b)?(a):(b))
#define abs(x) ((x)>0?(x):-(x))
#define constrain(amt,low,high) ((amt)<(low)?(low):((amt)>(high)?(high):(amt)))
#define round(x)     ((x)>=0?(long)((x)+0.5):(long)((x)-0.5))
#define radians(deg) ((deg)*DEG_TO_RAD)
#define degrees(rad) ((rad)*RAD_TO_DEG)
#define sq(x) ((x)*(x))

#define interrupts() sei()
#define noInterrupts() cli()

#define clockCyclesPerMicrosecond() ( F_CPU / 1000000L )
#define clockCyclesPerMillisecond() ( F_CPU / 1000L )
#define clockCyclesToMicroseconds(a) ( (a) / clockCyclesPerMicrosecond() )
#define microsecondsToClockCycles(a) ( (a) * clockCyclesPerMicrosecond() )

#define byte(w) ((uint8_t)(w))
#define lowByte(w) ((uint8_t) ((w) & 0xff))
#define highByte(w) ((uint8_t) ((w) >> 8))

//#define bitSet(value, bit) (__asm__("bset\t"))
//#define bitClear(value, bit) (__asm__("bres\t"))
#define bitRead(value, bit) (((value) >> (bit)) & 0x01)
#define bitSet(value, bit) ((value) |= (1UL << (bit)))
#define bitClear(value, bit) ((value) &= ~(1UL << (bit)))
#define bitWrite(value, bit, bitvalue) (bitvalue ? bitSet(value, bit) : bitClear(value, bit))

#define maskSet(value, mask) ((value) |= (mask))
#define maskClear(value, mask) ((value) &= ~(mask))


// avr-libc defines _NOP() since 1.6.2
#ifndef _NOP
#define _NOP() do { __asm__ volatile ("nop"); } while (0)
#endif

/* for SDCC this is supposed to be "__critical{" and "}", but up to
 * sdcc version 3.6.4 it is wrongly implemented. */
/* so geht es nicht:
#define BEGIN_CRITICAL		__asm__("push\tcc");__asm__("sim");
#define END_CRITICAL		__asm__("pop\tcc");
*/
#define BEGIN_CRITICAL		__critical {
#define END_CRITICAL		}
/* klappt:
#define BEGIN_CRITICAL
#define END_CRITICAL
*/



typedef unsigned int word;

#define bit(b) (1UL << (b))

typedef unsigned char boolean;
typedef unsigned char byte;
//typedef uint8_t byte;

void init(void);
void initVariant(void);		// weak

int atexit(void (*func)());	// __attribute__((weak));
void serialEvent(void);		// weak
extern unsigned char runSerialEvent;

void pinMode(uint8_t pin, uint8_t mode);
void digitalWrite(uint8_t pin, uint8_t val);
int digitalRead(uint8_t pin);
int analogRead(uint8_t pin);
void analogReference(uint8_t mode);
void analogWrite(uint8_t pin, int val);

unsigned long millis(void);
unsigned long micros(void);
void delay(unsigned long ms);
void delayMicroseconds(unsigned int us);
unsigned long pulseIn(uint8_t pin, uint8_t state, unsigned long timeout);
unsigned long pulseInLong(uint8_t pin, uint8_t state, unsigned long timeout);

void shiftOut(uint8_t dataPin, uint8_t clockPin, uint8_t bitOrder, uint8_t val);
uint8_t shiftIn(uint8_t dataPin, uint8_t clockPin, uint8_t bitOrder);

void attachInterrupt(uint8_t, void (*)(void), int mode);
void detachInterrupt(uint8_t);

void setup(void);
void loop(void);

// Get the bit location within the hardware port of the given virtual pin.
// This comes from the pins_*.c file for the active board configuration.

#define analogInPinToBit(P) (P)

// On the ATmega1280, the addresses of some of the port registers are
// greater than 255, so we can't store them in uint8_t's.
extern const uint16_t port_to_mode_PGM[];
extern const uint16_t PROGMEM port_to_input_PGM[];
extern const uint16_t PROGMEM port_to_output_PGM[];

extern const uint8_t PROGMEM digital_pin_to_port_PGM[];
// extern const uint8_t PROGMEM digital_pin_to_bit_PGM[];
extern const uint8_t PROGMEM digital_pin_to_bit_mask_PGM[];
extern const uint8_t PROGMEM digital_pin_to_timer_PGM[];

#ifdef SUPPORT_ALTERNATE_MAPPINGS
// helper function for STM8S: switch to the alternate pin functions
void alternateFunction(uint8_t val);
#endif

// Get the bit location within the hardware port of the given virtual pin.
// This comes from the pins_*.c file for the active board configuration.
// 
#define digitalPinToPort(P) ( digital_pin_to_port_PGM[(P)] )
#define digitalPinToBitMask(P) ( digital_pin_to_bit_mask_PGM[(P)] )
#define digitalPinToTimer(P) ( digital_pin_to_timer_PGM[(P)] )
#define analogInPinToBit(P) (P)
#define portOutputRegister(P) ( (volatile uint8_t *)( port_to_output_PGM[(P)]) )
#define portInputRegister(P) ( (volatile uint8_t *)( port_to_input_PGM[(P)]) )
#define portModeRegister(P) ( (volatile uint8_t *)( port_to_mode_PGM[(P)]) )

#define NOT_A_PIN 0
#define NOT_A_PORT 0

#define NOT_AN_INTERRUPT -1

#ifdef ARDUINO_MAIN
#define PA 1
#define PB 2
#define PC 3
#define PD 4
#define PE 5
#define PF 6
#define PG 7
#define PH 8
#define PJ 10
#define PK 11
#define PL 12
#endif

#include "pins_arduino.h"

enum {
    NOT_ON_TIMER = 0,
#ifdef NEED_TIMER_11_12
    TIMER11,
    TIMER12,
#endif
    TIMER13,
    TIMER14,
    TIMER21,
    TIMER22,
#ifdef NEED_TIMER_23
    TIMER23,
#endif
#ifdef NEED_TIMER_31_32
    TIMER31,
    TIMER32,
#endif
    NUM_TIMERS
};


//FIXME#include "WCharacter.h"
//FIXME#include "WString.h"
#include "HardwareSerial.h"

//uint16_t makeWord(uint16_t w);
//uint16_t makeWord(byte h, byte l);

//#define word(...) makeWord(__VA_ARGS__)

unsigned long pulseIn(uint8_t pin, uint8_t state, unsigned long timeout);
unsigned long pulseInLong(uint8_t pin, uint8_t state, unsigned long timeout);

void tone(uint8_t _pin, unsigned int frequency, unsigned long duration);
void noTone(uint8_t _pin);

// WMath prototypes
long random(long howbig);
long random_minmax(long howsmall, long howbig);
void randomSeed(unsigned long seed);
long map(long x, long in_min, long in_max, long out_min, long out_max);

inline unsigned int makeWord(unsigned char h, unsigned char l) { return (h << 8) | l; }


/*
 * The new interrupt numbers are a combination of the position in the
 * internal jump table (value in LSB) and the real STM8S-Interrupt number (MSB)
 */
#define INT_PORTA		( 0| (uint16_t)(ITC_IRQ_PORTA << 8))
#define INT_PORTB		( 1| (uint16_t)(ITC_IRQ_PORTB << 8))
#define INT_PORTC		( 2| (uint16_t)(ITC_IRQ_PORTC << 8))
#define INT_PORTD		( 3| (uint16_t)(ITC_IRQ_PORTD << 8))
#define INT_PORTE		( 4| (uint16_t)(ITC_IRQ_PORTE << 8))
#define INT_TIM1_CAPCOM		( 5| (uint16_t)(ITC_IRQ_TIM1_CAPCOM << 8))
#define INT_TIM1_OVF		( 6| (uint16_t)(ITC_IRQ_TIM1_OVF << 8))
#define INT_TIM2_CAPCOM		( 7| (uint16_t)(ITC_IRQ_TIM2_CAPCOM << 8))
#define INT_TIM2_OVF		( 8| (uint16_t)(ITC_IRQ_TIM2_OVF << 8))



#endif
