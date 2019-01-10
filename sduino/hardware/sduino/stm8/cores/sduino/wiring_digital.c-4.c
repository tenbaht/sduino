#include "wiring_digital.c.h"
/**
 * set the input or output mode of a pin
 */
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

