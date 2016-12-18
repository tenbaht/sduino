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

/*
uint8_t analog_reference = DEFAULT;

void analogReference(uint8_t mode)
{
	// can't actually set the register here because the default setting
	// will connect AVCC and the AREF pin, which would cause a short if
	// there's something connected to AREF.
	analog_reference = mode;
}

int analogRead(uint8_t pin)
{
	uint8_t low, high;

#if defined(analogPinToChannel)
#if defined(__AVR_ATmega32U4__)
	if (pin >= 18) pin -= 18; // allow for channel or pin numbers
#endif
	pin = analogPinToChannel(pin);
#elif defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)
	if (pin >= 54) pin -= 54; // allow for channel or pin numbers
#elif defined(__AVR_ATmega32U4__)
	if (pin >= 18) pin -= 18; // allow for channel or pin numbers
#elif defined(__AVR_ATmega1284__) || defined(__AVR_ATmega1284P__) || defined(__AVR_ATmega644__) || defined(__AVR_ATmega644A__) || defined(__AVR_ATmega644P__) || defined(__AVR_ATmega644PA__)
	if (pin >= 24) pin -= 24; // allow for channel or pin numbers
#else
	if (pin >= 14) pin -= 14; // allow for channel or pin numbers
#endif

#if defined(ADCSRB) && defined(MUX5)
	// the MUX5 bit of ADCSRB selects whether we're reading from channels
	// 0 to 7 (MUX5 low) or 8 to 15 (MUX5 high).
	ADCSRB = (ADCSRB & ~(1 << MUX5)) | (((pin >> 3) & 0x01) << MUX5);
#endif
  
	// set the analog reference (high two bits of ADMUX) and select the
	// channel (low 4 bits).  this also sets ADLAR (left-adjust result)
	// to 0 (the default).
#if defined(ADMUX)
#if defined(__AVR_ATtiny25__) || defined(__AVR_ATtiny45__) || defined(__AVR_ATtiny85__)
	ADMUX = (analog_reference << 4) | (pin & 0x07);
#else
	ADMUX = (analog_reference << 6) | (pin & 0x07);
#endif
#endif

	// without a delay, we seem to read from the wrong channel
	//delay(1);

#if defined(ADCSRA) && defined(ADCL)
	// start the conversion
	sbi(ADCSRA, ADSC);

	// ADSC is cleared when the conversion finishes
	while (bit_is_set(ADCSRA, ADSC));

	// we have to read ADCL first; doing so locks both ADCL
	// and ADCH until ADCH is read.  reading ADCL second would
	// cause the results of each conversion to be discarded,
	// as ADCL and ADCH would be locked when it completed.
	low  = ADCL;
	high = ADCH;
#else
	// we dont have an ADC, return 0
	low  = 0;
	high = 0;
#endif

	// combine the two bytes
	return (high << 8) | low;
}
*/

// Right now, PWM output only works on the pins with
// hardware support.  These are defined in the appropriate
// pins_*.c file.  For the rest of the pins, we default
// to digital output.
int HardwareSerial_write(uint8_t c);
void analogWrite(uint8_t pin, int val)
{
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

			case TIMER11:
				// connect pwm to pin on timer 1, channel 1
HardwareSerial_write('-');
				TIM1_OC1Init(
					TIM1_OCMODE_PWM2,
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
				break;
			case TIMER12:
				// connect pwm to pin on timer 1, channel 2
HardwareSerial_write('\\');
				TIM1_OC2Init(
					TIM1_OCMODE_PWM2,
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
				break;
			case TIMER13:
				// connect pwm to pin on timer 1, channel 3
HardwareSerial_write('|');
				TIM1_OC3Init(
					TIM1_OCMODE_PWM2,
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
				break;
			case TIMER14:
				// connect pwm to pin on timer 1, channel 4
HardwareSerial_write('/');
				TIM1_OC4Init(
					TIM1_OCMODE_PWM2,
					TIM1_OUTPUTSTATE_ENABLE,
					val,
					TIM1_OCPOLARITY_HIGH,
					TIM1_OCIDLESTATE_SET
				);
//				TIM1_OC4PreloadConfig(ENABLE);
//				TIM1_Cmd(ENABLE);
//				TIM1_CtrlPWMOutputs(ENABLE);
				break;

			case TIMER21:
//FIX: this channel still does not work. No idea why it is different.
				// connect pwm to pin on timer 2, channel 1
HardwareSerial_write('.');
#if 1
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
#if 0
				TIM2_OC2Init(
					TIM2_OCMODE_PWM1,
					TIM2_OUTPUTSTATE_ENABLE,
					val,
					TIM2_OCPOLARITY_HIGH
				);
#else
				TIM2->CCER1 |= TIM2_CCER1_CC2E;
				TIM2->CCMR2 = TIM2_OCMODE_PWM1 | TIM2_CCMR_OCxPE;
				TIM2->CCR2H = 0;
				TIM2->CCR2L = (uint8_t)(val);
#endif
				break;
			case TIMER23:
				// connect pwm to pin on timer 2, channel 3
#if 1
				TIM2_OC3Init(
					TIM2_OCMODE_PWM1,
					TIM2_OUTPUTSTATE_ENABLE,
					val,
					TIM2_OCPOLARITY_HIGH
				);
#else
				TIM2->CCER2 |= TIM2_CCER2_CC3E;
				TIM2->CCR3H = 0;
				TIM2->CCR3L = (uint8_t)(val);
#endif
				break;
/*
			// XXX fix needed for atmega8
			#if defined(TCCR0) && defined(COM00) && !defined(__AVR_ATmega8__)
			case TIMER0A:
				// connect pwm to pin on timer 0
				sbi(TCCR0, COM00);
				OCR0 = val; // set pwm duty
				break;
			#endif

			#if defined(TCCR0A) && defined(COM0A1)
			case TIMER0A:
				// connect pwm to pin on timer 0, channel A
				sbi(TCCR0A, COM0A1);
				OCR0A = val; // set pwm duty
				break;
			#endif
*/
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

