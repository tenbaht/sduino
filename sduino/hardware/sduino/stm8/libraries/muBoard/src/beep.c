/**
  \file timeout.c
   
  \author G. Icking-Konert
  \date 2013-11-22
  \version 0.1
   
  \brief implementation of timeouts based on 1ms clock
   
  implementation of timeouts based on 1ms clock (TIM4 ISR)
  Optional functionality via #define:
    - USE_TIM4_UPD_ISR: required for TIM4 ISR
*/

/*----------------------------------------------------------
    INCLUDE FILES
----------------------------------------------------------*/
#include "beep.h"

/*----------------------------------------------------------
    FUNCTIONS
----------------------------------------------------------*/

/**
  \fn uint8_t beep(uint8_t freq, uint16_t duration)
   
  \brief beeper control
   
  \param[in] freq       beeper frequency/state (1=1kHz, 2=2kHz, 4=4kHz, other=off)
  \param[in] duration   duration of signal (=0 --> set beeper and continue code execution immediately)
  
  \return  operation succeeded?
   
  control the on-board beeper with frequency/state and duration
*/
uint8_t beep(uint8_t freq, uint16_t duration) {
  
  // switch beeper frequency ond state (assume stable LS clock 128kHz)
  switch (freq) {
  
    case 1:
      BEEP_Init(BEEP_FREQUENCY_1KHZ);  // set 1KHz
      BEEP_Cmd(ENABLE);                // enable beeper
      break;
  
    case 2:
      BEEP_Init(BEEP_FREQUENCY_2KHZ);  // set 2KHz
      BEEP_Cmd(ENABLE);                // enable beeper
      break;
  
    case 4:
      BEEP_Init(BEEP_FREQUENCY_4KHZ);  // set 4KHz
      BEEP_Cmd(ENABLE);                // enable beeper
      break;
      
    default:
      BEEP_Cmd(DISABLE);  // disable beeper

  } // switch (state) 
      
  
  // if duration >0 -> wait time [ms], then disable beeper
  if (duration > 0) {
    delay(duration);
    BEEP_Cmd(DISABLE);
  }
  
  // no check -> ok
  return(SUCCESS);

} // beep


/*-----------------------------------------------------------------------------
    END OF MODULE
-----------------------------------------------------------------------------*/
