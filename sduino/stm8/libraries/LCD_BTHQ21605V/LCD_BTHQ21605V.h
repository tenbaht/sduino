/**
  \file LCD_BTHQ21605V.h
   
  \author G. Icking-Konert
  \date 2018-10-14
  \version 0.1
   
  \brief declaration of I2C routines for BTHQ21605V LCD
   
  declaration of LCD routines for 2x16 Batron BTHQ21605V-COG-FSRE-I2C (Farnell: 1220409).
  Control is via I2C, reset is via any digital pin (active high) 
*/

/*-----------------------------------------------------------------------------
    MODULE DEFINITION FOR MULTIPLE INCLUSION
-----------------------------------------------------------------------------*/
#ifndef _LCD_BTHQ21605V_H_
#define _LCD_BTHQ21605V_H_

#include <stdio.h>
#include <string.h>
#include "Arduino.h"
#include <I2C.h>


/*-----------------------------------------------------------------------------
    DECLARATION OF GLOBAL FUNCTIONS
-----------------------------------------------------------------------------*/

/// initialize LCD screen
void lcd_begin(uint8_t addrI2C, GPIO_TypeDef *rstPort, uint8_t rstPin);

/// clear LCD screen
void lcd_clear(void);

/// print string to LCD
uint8_t lcd_print_s(uint8_t line, uint8_t col, uint8_t *str);

/// print number to LCD display
uint8_t lcd_print_i(uint8_t line, uint8_t col, int value);


/*-----------------------------------------------------------------------------
    END OF MODULE DEFINITION FOR MULTIPLE INLUSION
-----------------------------------------------------------------------------*/
#endif // _LCD_BTHQ21605V_H_
