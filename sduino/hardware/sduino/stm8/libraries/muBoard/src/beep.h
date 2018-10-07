/**
  \file beep.h
   
  \author G. Icking-Konert
  \date 2013-11-22
  \version 0.1
   
  \brief declaration of timeouts based on 1ms clock
   
  declaration of timeouts based on 1ms clock (TIM4 ISR)
  Optional functionality via #define:
    - USE_TIM4_UPD_ISR: required for TIM4 ISR
*/

/*-----------------------------------------------------------------------------
    MODULE DEFINITION FOR MULTIPLE INCLUSION
-----------------------------------------------------------------------------*/
#ifndef _BEEPER_H_
#define _BEEPER_H_

#include <stdio.h>
#include <string.h>
//#include <inttypes.h>
#include "Arduino.h"

/*-----------------------------------------------------------------------------
    DECLARATION OF GLOBAL FUNCTIONS
-----------------------------------------------------------------------------*/

/// beeper control
uint8_t beep(uint8_t freq, uint16_t duration);


/*-----------------------------------------------------------------------------
    END OF MODULE DEFINITION FOR MULTIPLE INLUSION
-----------------------------------------------------------------------------*/
#endif // _BEEPER_H_
