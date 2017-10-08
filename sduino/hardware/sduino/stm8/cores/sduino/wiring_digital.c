/*
  wiring_digital.c - digital input and output functions
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

  Modified 28 September 2010 by Mark Sproul
*/

#define ARDUINO_MAIN
#include "wiring_private.h"
#include "pins_arduino.h"



/* this typedef is a SDCC wordaround.
 * Defining this a type instead of defining an array of (unsigned char *)
 * keeps SDCC from storing the array in the xinit segment and copy it to RAM
 * at runtime.
 */
typedef unsigned char *uc_p;

/* timer capture/compare mode register to control PWM mode */
const uc_p ccmrx[NUM_TIMERS]={
#ifdef NEED_TIMER_11_12
	TIM1->CCMR1,	/* for TIMER11 */
	TIM1->CCMR2,	/* for TIMER12 */
#endif
	TIM1->CCMR3,	/* for TIMER13 */
	TIM1->CCMR4,	/* for TIMER14 */
	TIM2->CCMR1,	/* for TIMER21 */
	TIM2->CCMR2,	/* for TIMER22 */
#ifdef NEED_TIMER_23
	TIM2->CCMR3,	/* for TIMER23 */
#endif
#ifdef NEED_TIMER_31_32
	TIM3->CCMR1,	/* for TIMER31 */
	TIM3->CCMR2	/* for TIMER32 */
#endif
};



/* arduino-style pinMode
void pinMode(uint8_t pin, uint8_t mode)
{
	uint8_t bit = digitalPinToBitMask(pin);
	uint8_t port = digitalPinToPort(pin);
	volatile uint8_t *reg, *out;

	if (port == NOT_A_PIN) return;

	reg = portModeRegister(port);
	out = portOutputRegister(port);

	if (mode == INPUT) { 
		BEGIN_CRITICAL
		*reg &= ~bit;
		*out &= ~bit;
		END_CRITICAL
	} else if (mode == INPUT_PULLUP) {
		BEGIN_CRITICAL
		*reg &= ~bit;
		*out |= bit;
		END_CRITICAL
	} else {
		BEGIN_CRITICAL
		*reg |= bit;
		END_CRITICAL
	}
}
*/

void pinMode(uint8_t pin, uint8_t mode)
{
	uint8_t bit = digitalPinToBitMask(pin);
	uint8_t port = digitalPinToPort(pin);
	volatile GPIO_TypeDef *gpio;

	if (port == NOT_A_PIN) return;

	gpio = (GPIO_TypeDef *) portOutputRegister(port);

	if (mode == INPUT) {
		BEGIN_CRITICAL
		gpio->CR2 &= ~bit;	// first: deactivate interrupt
		gpio->CR1 &= ~bit;	// release top side
		gpio->DDR &= ~bit;	// now set direction
		END_CRITICAL
	} else if (mode == INPUT_PULLUP) {
		BEGIN_CRITICAL
		gpio->CR2 &= ~bit;	// first: deactivate interrupt
		gpio->DDR &= ~bit;	// set direction before
		gpio->CR1 |=  bit;	// activating the pull up
		END_CRITICAL
	} else if (mode == OUTPUT_FAST) {// output push-pull, fast
		BEGIN_CRITICAL
		gpio->CR1 |=  bit;
		gpio->DDR |=  bit;	// direction before setting CR2 to
		gpio->CR2 |=  bit;	// avoid accidental interrupt
		END_CRITICAL
	} else if (mode == OUTPUT_OD_FAST) {	// output open drain, fast
		BEGIN_CRITICAL
		gpio->CR1 &= ~bit;
		gpio->DDR |=  bit;	// direction before setting CR2 to
		gpio->CR2 |=  bit;	// avoid accidental interrupt
		END_CRITICAL
	} else if (mode == OUTPUT_OD) {	// output open drain, slow
		BEGIN_CRITICAL
		gpio->CR1 &= ~bit;
		gpio->CR2 &= ~bit;
		gpio->DDR |=  bit;
		END_CRITICAL
	} else {			// output push-pull, slow
		BEGIN_CRITICAL
		gpio->CR1 |=  bit;
		gpio->CR2 &= ~bit;
		gpio->DDR |=  bit;
		END_CRITICAL
	}
}

/* using an array of pointers compiles way more efficient than doing simple
 * pointer arithmetics like
 *
	if (timer<TIMER21) {
		*(&TIM1->CCMR1 + (timer-TIMER11)) &= 0x8f;
	} else {
		*(&TIM2->CCMR1 + (timer-TIMER21)) &= 0x8f;
	}
 *
 * or a simple switch/case statement like
 *
	switch (timer)
	{
		case TIMER11:   TIM1->CCMR1 &= 0x8f;    break;
		case TIMER12:   TIM1->CCMR2 &= 0x8f;    break;
		case TIMER13:   TIM1->CCMR3 &= 0x8f;    break;
		case TIMER14:   TIM1->CCMR4 &= 0x8f;    break;
		case TIMER21:   TIM2->CCMR1 &= 0x8f;    break;
		case TIMER22:   TIM2->CCMR2 &= 0x8f;    break;
		case TIMER23:   TIM2->CCMR3 &= 0x8f;    break;
	}
 *
 * The most efficient way is this:
 *
#define T1_BASE 0x5258
#define T2_BASE 0x5307
	uint8_t *reg = T1_BASE-1;
	if (timer>4) reg+= (T2_BASE - T1_BASE);
	reg[timer] &= ~TIM1_CCMR_OCM;
 *
 * Unfortunatly, SDCC can't figure out the values TIM1->CCMR1 und TIM->CCMR2
 * early enough in the compile process, so the adresses have to be hardcoded
 * into the code.
 *
 * SDCC is really, really not good in optimizing its code.
 */
static void turnOffPWM(uint8_t timer)
{
	*((unsigned char *) ccmrx[timer-1]) &= ~TIM1_CCMR_OCM;
}


void digitalWrite(uint8_t pin, uint8_t val)
{
	uint8_t timer = digitalPinToTimer(pin);
	uint8_t bit = digitalPinToBitMask(pin);
	uint8_t port = digitalPinToPort(pin);
	volatile uint8_t *out;

	if (port == NOT_A_PIN) return;

	// If the pin that support PWM output, we need to turn it off
	// before doing a digital write.
	if (timer != NOT_ON_TIMER) turnOffPWM(timer);

	out = portOutputRegister(port);

	BEGIN_CRITICAL

	if (val == LOW) {
		*out &= ~bit;
	} else {
		*out |= bit;
	}

	END_CRITICAL
}

int digitalRead(uint8_t pin)
{
	uint8_t timer = digitalPinToTimer(pin);
	uint8_t bit = digitalPinToBitMask(pin);
	uint8_t port = digitalPinToPort(pin);

	if (port == NOT_A_PIN) return LOW;

	// If the pin that support PWM output, we need to turn it off
	// before getting a digital reading.
	if (timer != NOT_ON_TIMER) turnOffPWM(timer);

	if (*portInputRegister(port) & bit) return HIGH;
	return LOW;
}
