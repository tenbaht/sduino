/* -*- mode: jde; c-basic-offset: 2; indent-tabs-mode: nil -*- */

/*
  Part of the Wiring project - http://wiring.uniandes.edu.co

  Copyright (c) 2004-05 Hernando Barragan

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
  
  Modified 24 November 2006 by David A. Mellis
  Modified 1 August 2010 by Mark Sproul
  Modified 2 February 2017 for use with sduino/STM8 by Michael Mayer
*/

//#include <inttypes.h>
//#include <stdio.h>

#include "wiring_private.h"

static void nothing(void) {
}

static volatile voidFuncPtr intFunc[EXTERNAL_NUM_INTERRUPTS] = {
#if EXTERNAL_NUM_INTERRUPTS > 9
    #warning There are more than 9 external interrupts. Some callbacks may not be initialized.
    nothing,
#endif
#if EXTERNAL_NUM_INTERRUPTS > 8
    nothing,
#endif
#if EXTERNAL_NUM_INTERRUPTS > 7
    nothing,
#endif
#if EXTERNAL_NUM_INTERRUPTS > 6
    nothing,
#endif
#if EXTERNAL_NUM_INTERRUPTS > 5
    nothing,
#endif
#if EXTERNAL_NUM_INTERRUPTS > 4
    nothing,
#endif
#if EXTERNAL_NUM_INTERRUPTS > 3
    nothing,
#endif
#if EXTERNAL_NUM_INTERRUPTS > 2
    nothing,
#endif
#if EXTERNAL_NUM_INTERRUPTS > 1
    nothing,
#endif
#if EXTERNAL_NUM_INTERRUPTS > 0
    nothing
#endif
};
// volatile static voidFuncPtr twiIntFunc;

// some small CPUs don't require support for pin change interrupts on
// port E. This allows a small optimization for these CPUs.
#if defined(STM8S001) || defined(STM8S003) || defined(STM8S103)
# define USE_PORT_E 0
#else
# define USE_PORT_E 1
#endif


void attachInterrupt(uint8_t pin, void (*userFunc)(void), int mode)
{
  uint8_t portNum;

  if (pin >= NUM_DIGITAL_PINS) return;
  portNum = digitalPinToPort(pin);

  if (portNum <= PE)
  {
    uint8_t *cr2 = portModeRegister(portNum)+2;

    portNum--;
    intFunc[portNum] = userFunc;

    // Configure the interrupt mode (trigger on falling and low input, any
    // change, rising edge, or falling edge). The mode constants were chosen
    // to correspond to the configuration bits in the hardware register, so
    // we simply shift the mode into place.

    // Write access to EXTI_CRx is only possible when I0 and I1 in CCR = 11
    // (see RM0016, Ch. 6.9.3). This requires a SIM instruction using
    //FIXME: should be {push cc; sim}/{pop cc}
    __critical{
#if USE_PORT_E
      if ( portNum > 3 )
      {
        // There is a hardcoded maximum of 5 external interrupts here
        // since even the bigger MCUs don't support more than this.
        EXTI->CR2 &= ~3;
        EXTI->CR2 |= (uint8_t)(mode&3);
      }
      else
#endif
      {
        EXTI->CR1 &= (uint8_t)(~( 3 << (2*portNum) ));
        EXTI->CR1 |= (uint8_t)((uint8_t)(mode&3) << (2*portNum));
      }
    }

    // Finally, enable the PC interrupt for the pin.
    *cr2 |= digitalPinToBitMask(pin);
  }
}


void detachInterrupt(uint8_t pin)
{
  uint8_t portNum;

  if (pin >= NUM_DIGITAL_PINS) return;

  portNum = digitalPinToPort(pin);

  if(portNum > EXTERNAL_NUM_INTERRUPTS) return;

  {
    // Disable the PC interrupt for the pin.
    portModeRegister(portNum)[2] &= ~digitalPinToBitMask(pin);	// CR2
    // no need to remove the pointer to the IRQ function in intFunc[]
//    uint8_t *ddr = portModeRegister(portNum);	// DDR
//    ddr[2] &= ~digitalPinToBitMask(pin);	// CR2
  }
}

/*
###
  // unsigned, to make sure that (NOT_A_PORT-1) wraps over to 255
  uint8_t interruptNum = digitalPinToPort(pin)-1;

  // this catches NOT_A_PORT as well, because it wrapped over to 255
  if(interruptNum < EXTERNAL_NUM_INTERRUPTS) {
    // Disable the PC interrupt for the pin.
    GPIOx->CR2 &= ~digitalPinToBitMask(pin);

    if ((GPIOx->CR2 & ~GPIOx->DDR)==0) {
      // no other PC interrupt active for this port, disable the IRQ function
      intFunc[interruptNum] = nothing;
    }
}
*/


/*
void attachInterruptTwi(void (*userFunc)(void) ) {
  twiIntFunc = userFunc;
}
*/

// define as __naked to avoid the nonsense "clr a/dix x,a" prolog
#define IMPLEMENT_ISR(vect, interrupt) \
 void vect(void) __interrupt((interrupt)>>8) __naked { \
    intFunc[(interrupt)&0xff](); \
    __asm__("iret"); \
  }
/* would be great, but need to evaluate the #defines first
#define IMPLEMENT_ISR(vect, interrupt) \
 void vect(void) __interrupt((interrupt) & 0xff) __naked { \
   __asm \
    call	[_intFunc+2*(interrupt)] \
    iret \
    __endasm; \
  }
*/

IMPLEMENT_ISR(EXTI_PORTA_IRQHandler,		INT_PORTA) /* EXTI PORTA */
IMPLEMENT_ISR(EXTI_PORTB_IRQHandler,		INT_PORTB) /* EXTI PORTB */
IMPLEMENT_ISR(EXTI_PORTC_IRQHandler,		INT_PORTC) /* EXTI PORTC */
IMPLEMENT_ISR(EXTI_PORTD_IRQHandler,		INT_PORTD) /* EXTI PORTD */
IMPLEMENT_ISR(EXTI_PORTE_IRQHandler,		INT_PORTE) /* EXTI PORTE */
IMPLEMENT_ISR(TIM1_CAP_COM_IRQHandler,		INT_TIM1_CAPCOM)
//IMPLEMENT_ISR(TIM1_UPD_OVF_TRG_BRK_IRQHandler,	INT_TIM1_OVF)
//IMPLEMENT_ISR(TIM2_CAP_COM_IRQHandler,		INT_TIM2_CAPCOM)
//IMPLEMENT_ISR(TIM2_UPD_OVF_TRG_BRK_IRQHandler,	INT_TIM2_OVF)


/*
ISR(TWI_vect) {
  if(twiIntFunc)
    twiIntFunc();
}
*/

