/**
  \file LCD_BTHQ21605V.c
   
  \author G. Icking-Konert
  \date 2018-10-14
  \version 0.1
   
  \brief implementation of I2C routines for BTHQ21605V LCD
   
  implementation of LCD routines for 2x16 Batron BTHQ21605V-COG-FSRE-I2C (Farnell: 1220409).
  Control is via I2C with fixed address 0x3B; Reset is via any digital pin (active high) 
*/

/*----------------------------------------------------------
    INCLUDE FILES
----------------------------------------------------------*/
#include "LCD_BTHQ21605V.h"


/*----------------------------------------------------------
    module variables
----------------------------------------------------------*/
uint8_t   m_addrI2C;	// I2C address (set via lcd_begin)


/*----------------------------------------------------------
    FUNCTIONS
----------------------------------------------------------*/

/**
  \fn void lcd_begin(uint8_t addrI2C, GPIO_TypeDef *rstPort, uint8_t rstPin)
   
  \brief reset LCD screen
  
  \param[in]  addrI2C   I2C address of LCD
  \param[in]  rstPort   port of reset pin, e.g GPIOE
  \param[in]  rstPin    pin number of reset pin [0..7]

  init I2C module on controller, reset LCD and clear screen
*/
void lcd_begin(uint8_t addrI2C, GPIO_TypeDef *rstPort, uint8_t rstPin) {
  
  // stor I2C address for use below
  m_addrI2C = addrI2C;

  // init I2C module
  I2C_begin();

  // LCD reset via PE3 high
  uint8_t mask = (1 << rstPin);
  rstPort->DDR |= mask;
  rstPort->CR1 |= mask;
  rstPort->CR2 |= mask;
  rstPort->ODR |= mask;
  delay(10);
  rstPort->ODR &= ~mask;
  delay(10);

  // clear screen
  lcd_clear();
  
} // lcd_begin



/**
  \fn void lcd_clear(void)
   
  \brief clear LCD screen
  
  clear screen by filling with space
*/
void lcd_clear() {
  
  char str[20] = "                   ";  
  lcd_print_s(1, 1, str);
  lcd_print_s(2, 1, str);
  
} // lcd_clear



/**
  \fn uint8_t lcd_print_s(uint8_t line, uint8_t col, uint8_t *str)
   
  \brief print string to LCD
  
  \param[in]  line    line to print to (1 or 2)
  \param[in]  col     column to start at (1..16)
  \param[in]  str     string to print

  \return operation successful?

  print string to 2x16 LCD display
  
*/
uint8_t lcd_print_s(uint8_t line, uint8_t col, uint8_t *str) {

  uint8_t   s[20];
  uint8_t   i, j;
  uint8_t   result;

  //////
  // config display
  //////
  i = 0;
  s[i++] = 0x34;                   // command 'function set'
  s[i++] = 0x0C;                   // command 'display on'
  s[i++] = 0x06;                   // command 'entry mode'
  if (line == 1)
    s[i++] = (uint8_t)(0x80 + col - 1); // DRAM adress offset for line 1 + column
  else
    s[i++] = (uint8_t)(0xC0 + col - 1); // DRAM adress offset for line 2
  
  // send data (with timeout). Register 0x00 -> 'config mode'
  result = I2C_write_sn(m_addrI2C, 0x00, s, i);


  //////
  // send output string
  //////

  // copy string
  i = 0;
  for (j=0; (j<strlen(str)) && (i<20); j++) {
  
    // replace TAB, CR, LF, BEL with SPC
    if ((str[j] == '\t') || (str[j] == '\r') || (str[j] == '\n') || (str[j] == 7))
      s[i++] = 0xA0;
    
    // convert ASCII to lcd char set (+128)
    else
      s[i++] = (uint8_t)(str[j] | 0x80);
  
  }
  
  // send data. Register 0x40 -> 'write data'
  result |= I2C_write_sn(m_addrI2C, 0x40, s, i);
  
  // return status
  return(result);

} // lcd_print_s



/**
  \fn uint8_t lcd_print_i(uint8_t line, uint8_t col, int value)
   
  \brief print number to LCD display
  
  \param[in]  line      line to print to (1 or 2)
  \param[in]  col       column to start at (1..16)
  \param[in]  value     number to print

  \return operation successful?

  print number to 2x16 LCD display
  
*/
uint8_t lcd_print_i(uint8_t line, uint8_t col, int value) {

  uint8_t  str[20], result;

  // convert number to string
  sprintf(str, "%ld", (long) value);
  
  // output to LCD
  result = lcd_print_s(line, col, str);
  
  // return status
  return(result);

} // lcd_print_i


/*-----------------------------------------------------------------------------
    END OF MODULE
-----------------------------------------------------------------------------*/
