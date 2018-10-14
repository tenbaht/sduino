/**
  \file muBoard_beep.h
   
  \author G. Icking-Konert
  \date 2018-10-14
  \version 0.1
   
  \brief declaration of beeper routine
   
  declaration of routines for beeper control
*/

/*-----------------------------------------------------------------------------
    MODULE DEFINITION FOR MULTIPLE INCLUSION
-----------------------------------------------------------------------------*/
#ifndef _BEEPER_H_
#define _BEEPER_H_

#include <stdio.h>
#include <string.h>
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
