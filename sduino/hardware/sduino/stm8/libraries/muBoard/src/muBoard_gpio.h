/**
  \file muBoard_gpio.h
   
  \author G. Icking-Konert
  \date 2018-10-14
  \version 0.1
   
  \brief declaration of GPIO routines
   
  declaration of routines for GPIO control outside normal digital pins
*/

/*-----------------------------------------------------------------------------
    MODULE DEFINITION FOR MULTIPLE INCLUSION
-----------------------------------------------------------------------------*/
#ifndef _GPIO_H_
#define _GPIO_H_

#include <stdio.h>
#include <string.h>
#include "Arduino.h"

// pin configurations for below pinMode(). For details see 'stm8as.h'
#define  MB_INPUT                    0x00     ///< configure pin as: input, float, no port interrupt
#define  MB_INPUT_INTERRUPT          0x01     ///< configure pin as: input, float, with port interrupt
#define  MB_INPUT_PULLUP             0x02     ///< configure pin as: input, pull-up, no port interrupt
#define  MB_INPUT_PULLUP_INTERRUPT   0x03     ///< configure pin as: input, pull-up, with port interrupt
#define  MB_OUTPUT_OPENDRAIN_SLOW    0x04     ///< configure pin as: output, open-drain, slow (2MHz)
#define  MB_OUTPUT_OPENDRAIN         0x05     ///< configure pin as: output, open-drain, fast (10MHz)
#define  MB_OUTPUT_SLOW              0x06     ///< configure pin as: output, push-pull, slow (2MHz)
#define  MB_OUTPUT                   0x07     ///< configure pin as: output, push-pull, fast (10MHz)

#define pinHigh(pPort,pin)          ((pPort)->ODR  |= (0x01<<pin))      ///< set pin output state high
#define pinLow(pPort,pin)           ((pPort)->ODR  &= ~(0x01<<pin))     ///< set pin output state high
#define pinToggle(pPort,pin)        ((pPort)->ODR  ^= (0x01<<pin))      ///< toggle pin output state
#define pinSet(pPort,pin,state)     (state ? pinHigh(pPort,pin) : pinLow(pPort,pin)) ///< set pin output state
#define pinRead(pPort,pin)          (((pPort)->IDR & (0x01<<pin))>>pin) ///< read pin input state


/*-----------------------------------------------------------------------------
    DECLARATION OF GLOBAL FUNCTIONS
-----------------------------------------------------------------------------*/

/// initialize all muBoard GPIO pins
void     gpioInit(void);

/// set pin mode (similar to pinMode)
void     gpioMode(GPIO_TypeDef *port, uint8_t pin, uint8_t mode);


/*-----------------------------------------------------------------------------
    END OF MODULE DEFINITION FOR MULTIPLE INLUSION
-----------------------------------------------------------------------------*/
#endif // _GPIO_H_
