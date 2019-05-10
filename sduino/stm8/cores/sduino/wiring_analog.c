/*
  wiring_analog.c - analog input and output
  Adopted for use with the STM8S.
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
  Modified 17 December 2016 by Michael Mayer
*/

#include "wiring_private.h"
#include "pins_arduino.h"


#ifndef NO_ANALOG_IN
/*
uint8_t analog_reference = DEFAULT;

void analogReference(uint8_t mode)
{
	// can't actually set the register here because the default setting
	// will connect AVCC and the AREF pin, which would cause a short if
	// there's something connected to AREF.
	analog_reference = mode;
}
*/

int analogRead(uint8_t pin)
{
	uint8_t low, high;

#if defined(analogPinToChannel)
	pin = analogPinToChannel(pin);
#else
	// default case: use the last (NUM_ANALOG_INPUTS) pins for analog
	if (pin >= NUM_ANALOG_INPUTS) {
		pin -= NUM_DIGITAL_INPUTS - NUM_ANALOG_INPUTS;
	}
#endif
	// note there is no range check anymore

#ifdef USE_SPL
	// using spl functions:
	#if defined(ADC1)
		ADC1_ConversionConfig(ADC1_CONVERSIONMODE_SINGLE, pin & 15, ADC1_ALIGN_RIGHT);
		ADC1_PrescalerConfig(ADC1_PRESSEL_FCPU_D18);
		ADC1_ExternalTriggerConfig(ADC1_EXTTRIG_TIM, DISABLE);
		ADC1_SchmittTriggerConfig(pin, DISABLE);
	#endif
	#if defined(ADC2)
		ADC2_ConversionConfig(ADC2_CONVERSIONMODE_SINGLE, pin & 15, ADC2_ALIGN_RIGHT);
		ADC2_PrescalerConfig(ADC2_PRESSEL_FCPU_D18);
		ADC2_ExternalTriggerConfig(ADC2_EXTTRIG_TIM, DISABLE);
		ADC2_SchmittTriggerConfig(pin, DISABLE);
	#endif

#else
	// direct register access:
	// select channel
	#if defined(ADC1)
		ADC1->CSR = pin & 15;
		bitSet(ADC1->CR2, 3);	// right align
	#endif
	#if defined(ADC2)
		ADC2->CSR = pin & 15;
		bitSet(ADC2->CR2, 3);	// right align
	#endif
#endif
	// first write turns on the ADC
	#if defined(ADC1)
		//bitSet(ADC1->CR1, 0);
		ADC1->CR1 |= ADC1_CR1_ADON;

		// second write starts the conversion
		//bitSet(ADC1->CR1, 0);
		ADC1->CR1 |= ADC1_CR1_ADON;

		// EOC is set when the conversion finishes
		while (! (ADC1->CSR & ADC1_FLAG_EOC));
		ADC1->CSR &= ~(ADC1_FLAG_EOC);	// important! clear the flag

		// in right align mode we have to read DRL first
		low  = ADC1->DRL;
		high = ADC1->DRH;

		// turn off the ADC, free the pin for digital use again
		bitClear(ADC1->CR1, 0);
	#endif
	#if defined(ADC2)
		//bitSet(ADC2->CR1, 0);
		ADC2->CR1 |= ADC2_CR1_ADON;

		// second write starts the conversion
		//bitSet(ADC2->CR1, 0);
		ADC2->CR1 |= ADC2_CR1_ADON;

		// EOC is set when the conversion finishes
		while (! (ADC2->CSR & ADC2_CSR_EOC));
		ADC2->CSR &= ~(ADC2_CSR_EOC);	// important! clear the flag

		// in right align mode we have to read DRL first
		low  = ADC2->DRL;
		high = ADC2->DRH;

		// turn off the ADC, free the pin for digital use again
		bitClear(ADC2->CR1, 0);
	#endif

	// combine the two bytes
	return (high << 8) | low;
}
#endif //ifndef NO_ANALOG_IN


#ifndef NO_ANALOG_OUT
// Right now, PWM output only works on the pins with
// hardware support.  These are defined in the appropriate
// pins_*.c file.  For the rest of the pins, we default
// to digital output.

//int HardwareSerial_write(uint8_t c);
void analogWrite(uint8_t pin, int val)
{
	unsigned char tmp;	// only to help SDCC to keep it in a register

	// We need to make sure the PWM output is enabled for those pins
	// that support it, as we turn it off when digitally reading or
	// writing with them.  Also, make sure the pin is in output mode
	// for consistenty with Wiring, which doesn't require a pinMode
	// call for the analog output pins.
	pinMode(pin, OUTPUT);
	if (val == 0)
	{
		digitalWrite(pin, LOW);
	}
	else if (val >= 255)
	{
		digitalWrite(pin, HIGH);
	}
	else
	{
		switch(digitalPinToTimer(pin))
		{
#ifdef NEED_TIMER_11_12
			case TIMER11:
				// connect pwm to pin on timer 1, channel 1
//				alternateFunction(1);
#ifdef USE_SPL
				TIM1_OC1Init(
					TIM1_OCMODE_PWM1,
					TIM1_OUTPUTSTATE_ENABLE,
					TIM1_OUTPUTNSTATE_DISABLE,
					val,
					TIM1_OCPOLARITY_HIGH,
					TIM1_OCNPOLARITY_HIGH,
					TIM1_OCIDLESTATE_SET,
					TIM1_OCNIDLESTATE_SET
				);
//				TIM1_OC1PreloadConfig(ENABLE);
//				TIM1_Cmd(ENABLE);
//				TIM1_CtrlPWMOutputs(ENABLE);
#else
				// write MSB first, DO NOT USE ldw instruction!
				tmp = TIM1->CCER1 & (uint8_t)(~(TIM1_CCER1_CC1E | TIM1_CCER1_CC1P));
				TIM1->CCER1 = tmp | TIM2_CCER1_CC1E;
				TIM1->CCMR1 = TIM1_OCMODE_PWM1 | TIM1_CCMR_OCxPE;
				TIM1->CCR1H = 0;
				TIM1->CCR1L = (uint8_t)(val);
#endif
				break;
			case TIMER12:
				// connect pwm to pin on timer 1, channel 2
//				alternateFunction(1);
#ifdef USE_SPL
				TIM1_OC2Init(
					TIM1_OCMODE_PWM1,
					TIM1_OUTPUTSTATE_ENABLE,
					TIM1_OUTPUTNSTATE_DISABLE,
					val,
					TIM1_OCPOLARITY_HIGH,
					TIM1_OCNPOLARITY_HIGH,
					TIM1_OCIDLESTATE_SET,
					TIM1_OCNIDLESTATE_SET
				);
//				TIM1_OC2PreloadConfig(ENABLE);
//				TIM1_Cmd(ENABLE);
//				TIM1_CtrlPWMOutputs(ENABLE);
#else
				// write MSB first, DO NOT USE ldw instruction!
				tmp = TIM1->CCER1 & (uint8_t)(~(TIM1_CCER1_CC2E | TIM1_CCER1_CC2P));
				TIM1->CCER1 = tmp | TIM2_CCER1_CC2E;
				TIM1->CCMR2 = TIM1_OCMODE_PWM1 | TIM1_CCMR_OCxPE;
				TIM1->CCR2H = 0;
				TIM1->CCR2L = (uint8_t)(val);
#endif // if use_spl
				break;
#endif // if NEED_TIMER_11_12
			case TIMER13:
				// connect pwm to pin on timer 1, channel 3
#ifdef USE_SPL
				TIM1_OC3Init(
					TIM1_OCMODE_PWM1,
					TIM1_OUTPUTSTATE_ENABLE,
					TIM1_OUTPUTNSTATE_DISABLE,
					val,
					TIM1_OCPOLARITY_HIGH,
					TIM1_OCNPOLARITY_HIGH,
					TIM1_OCIDLESTATE_SET,
					TIM1_OCNIDLESTATE_SET
				);
//				TIM1_OC3PreloadConfig(ENABLE);
//				TIM1_Cmd(ENABLE);
//				TIM1_CtrlPWMOutputs(ENABLE);
#else
				// write MSB first, DO NOT USE ldw instruction!
//				tmp = TIM1->CCER2 & (uint8_t)(~(TIM1_CCER2_CC3E | TIM1_CCER2_CC3P));
//				TIM1->CCER2 = tmp | TIM2_CCER2_CC3E;
				TIM1->CCER2 |= TIM2_CCER2_CC3E;
				TIM1->CCMR3 = TIM1_OCMODE_PWM1 | TIM1_CCMR_OCxPE;
				TIM1->CCR3H = 0;
				TIM1->CCR3L = (uint8_t)(val);
#endif
				break;
			case TIMER14:
				// connect pwm to pin on timer 1, channel 4
#ifdef USE_SPL
				TIM1_OC4Init(
					TIM1_OCMODE_PWM1,
					TIM1_OUTPUTSTATE_ENABLE,
					val,
					TIM1_OCPOLARITY_HIGH,
					TIM1_OCIDLESTATE_SET
				);
#else
				// write MSB first, DO NOT USE ldw instruction!
				tmp = TIM1->CCER2 & (uint8_t)(~(TIM1_CCER2_CC4E | TIM1_CCER2_CC4P));
				TIM1->CCER2 = tmp | TIM1_CCER2_CC4E;
				TIM1->CCMR4 = TIM1_OCMODE_PWM1 | TIM1_CCMR_OCxPE;
				TIM1->CCR4H = 0;
				TIM1->CCR4L = (uint8_t)(val);
#endif
				break;
			case TIMER21:
				// connect pwm to pin on timer 2, channel 1
#ifdef USE_SPL
				TIM2_OC1Init(
					TIM2_OCMODE_PWM1,
					TIM2_OUTPUTSTATE_ENABLE,
					val,
					TIM2_OCPOLARITY_HIGH
				);
				TIM2_OC1PreloadConfig(ENABLE);
#else
				// eigentlich würde dies genügen:
				// write MSB first, DO NOT USE ldw instruction!
				TIM2->CCER1 |= TIM2_CCER1_CC1E;
				TIM2->CCMR1 = TIM2_OCMODE_PWM1 | TIM2_CCMR_OCxPE;
				TIM2->CCR1H = 0;
				TIM2->CCR1L = (uint8_t)(val);
#endif
				break;
			case TIMER22:
				// connect pwm to pin on timer 2, channel 2
#ifdef USE_SPL
				TIM2_OC2Init(
					TIM2_OCMODE_PWM1,
					TIM2_OUTPUTSTATE_ENABLE,
					val,
					TIM2_OCPOLARITY_HIGH
				);
#else
//				tmp = TIM2->CCER1 & (uint8_t)(~(TIM2_CCER1_CC2E | TIM2_CCER1_CC2P));
//				TIM2->CCER1 = tmp | TIM2_CCER1_CC2E;
				TIM2->CCMR2 = TIM2_OCMODE_PWM1 | TIM2_CCMR_OCxPE;
				TIM2->CCER1 |= TIM2_CCER1_CC2E;
				TIM2->CCR2H = 0;
				TIM2->CCR2L = (uint8_t)(val);
#endif
				break;
#ifdef NEED_TIMER_23
			case TIMER23:
				// connect pwm to pin on timer 2, channel 3
#ifdef USE_SPL
				TIM2_OC3Init(
					TIM2_OCMODE_PWM1,
					TIM2_OUTPUTSTATE_ENABLE,
					val,
					TIM2_OCPOLARITY_HIGH
				);
#else
				// write MSB first, DO NOT USE ldw instruction!
				tmp = TIM2->CCER2 & (uint8_t)(~(TIM2_CCER2_CC3E | TIM2_CCER2_CC3P));
				TIM2->CCER2 = tmp | TIM2_CCER2_CC3E;
//				TIM2->CCER2 |= TIM2_CCER2_CC3E;
				TIM2->CCMR3 = TIM2_OCMODE_PWM1 | TIM2_CCMR_OCxPE;
				TIM2->CCR3H = 0;
				TIM2->CCR3L = (uint8_t)(val);
#endif
				break;
#endif // ifdef NEED_TIMER_23
#ifdef NEED_TIMER_31_32
			case TIMER31:
				// connect pwm to pin on timer 3, channel 1
				// eigentlich würde dies genügen:
				// write MSB first, DO NOT USE ldw instruction!
				TIM3->CCER1 |= TIM3_CCER1_CC1E;
				TIM3->CCMR1 = TIM3_OCMODE_PWM1 | TIM3_CCMR_OCxPE;
				TIM3->CCR1H = 0;
				TIM3->CCR1L = (uint8_t)(val);
				break;
			case TIMER32:
				// connect pwm to pin on timer 3, channel 2
				TIM3->CCMR2 = TIM3_OCMODE_PWM1 | TIM3_CCMR_OCxPE;
				TIM3->CCER1 |= TIM3_CCER1_CC2E;
				TIM3->CCR2H = 0;
				TIM3->CCR2L = (uint8_t)(val);
				break;
#endif // ifdef NEED_TIMER_31_32
			case NOT_ON_TIMER:
			default:
				if (val < 128) {
					digitalWrite(pin, LOW);
				} else {
					digitalWrite(pin, HIGH);
				}
		}
	}
}
#endif // ifndef NO_ANALOG_OUT
