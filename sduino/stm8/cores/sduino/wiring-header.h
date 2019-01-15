/*
  wiring.c - Partial implementation of the Wiring API for the ATmega8.
  Part of Arduino - http://www.arduino.cc/

  Copyright (c) 2005-2006 David A. Mellis

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

#include "wiring_private.h"


/* calculate the best prescaler and timer period for TIM4 for millis()
 *
 * The precaler factor could be calculated from the prescaler enum by
 * bit shifting, but we want to be independent from possible irregular
 * definitions for the chose CPU type.
 */
#if (clockCyclesPerMillisecond() < 256)
# define T4PRESCALER	TIM4_PRESCALER_1
# define T4PRESCALER_FACTOR	1
#elif (clockCyclesPerMillisecond() < 512)
# define T4PRESCALER	TIM4_PRESCALER_2
# define T4PRESCALER_FACTOR	2
#elif (clockCyclesPerMillisecond() < 1024)
# define T4PRESCALER	TIM4_PRESCALER_4
# define T4PRESCALER_FACTOR	4
#elif (clockCyclesPerMillisecond() < 2048)
# define T4PRESCALER	TIM4_PRESCALER_8
# define T4PRESCALER_FACTOR	8
#elif (clockCyclesPerMillisecond() < 4096)
# define T4PRESCALER	TIM4_PRESCALER_16
# define T4PRESCALER_FACTOR	16
#elif (clockCyclesPerMillisecond() < 8192)
# define T4PRESCALER	TIM4_PRESCALER_32
# define T4PRESCALER_FACTOR	32
#elif (clockCyclesPerMillisecond() < 16384)
# define T4PRESCALER	TIM4_PRESCALER_64
# define T4PRESCALER_FACTOR	64
#elif (clockCyclesPerMillisecond() < 32768)
# define T4PRESCALER	TIM4_PRESCALER_128
# define T4PRESCALER_FACTOR	128
#else
#error "could not calculate a valid prescaler für TIM4"
#endif
#define T4PERIOD	(clockCyclesPerMillisecond()/T4PRESCALER_FACTOR)

// the prescaler is set so that timer4 ticks every 64 clock cycles, and the
// the overflow handler is called every 250 ticks.
# define MICROSECONDS_PER_TIMER0_OVERFLOW (F_CPU/(T4PRESCALER_FACTOR*T4PERIOD))
//#define MICROSECONDS_PER_TIMER0_OVERFLOW (clockCyclesToMicroseconds(64 * 250))

// the whole number of milliseconds per timer4 overflow
#define MILLIS_INC (MICROSECONDS_PER_TIMER0_OVERFLOW / 1000)

// the fractional number of milliseconds per timer4 overflow. we shift right
// by three to fit these numbers into a byte. (for the clock speeds we care
// about - 8 and 16 MHz - this doesn't lose precision.)
#define FRACT_INC ((MICROSECONDS_PER_TIMER0_OVERFLOW % 1000) >> 3)
#define FRACT_MAX (1000 >> 3)

extern volatile unsigned long timer4_overflow_count;// = 0;
extern volatile unsigned long timer4_millis;// = 0;
