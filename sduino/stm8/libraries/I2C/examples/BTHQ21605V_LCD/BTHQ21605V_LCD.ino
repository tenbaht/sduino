/*******************************************
 Sample sketch that prints text to a 2x16 LCD
 (Batron BTHQ21605V-COG-FSRE-I2C; Farnell: 1220409)
 via I2C.
 Connect I2C SCL/SDA pins, and RST to any digital pin.
 *******************************************/

#include <I2C.h>

#define ADDR_LCD    59

void reset_lcd(GPIO_TypeDef *port, uint8_t pin);
void clear_lcd();
uint8_t print_lcd_s(uint8_t line, uint8_t col, uint8_t *str);
uint8_t print_lcd_i(uint8_t line, uint8_t col, int num);



/**
  \fn void reset_lcd(GPIO_TypeDef *port, uint8_t pin)
   
  \brief reset LCD screen
  
  \param  port    port of reset pin, e.g GPIOE
  \param  port    pin number of reset pin [0..7]

  init I2C module on controller, reset LCD and clear screen
*/
void reset_lcd(GPIO_TypeDef *port, uint8_t pin) {
  
  // init I2C module
  I2C_begin();

  // LCD reset via PE3 high
  uint8_t mask = (1 << pin);
  port->DDR |= mask;
  port->CR1 |= mask;
  port->CR2 |= mask;
  port->ODR |= mask;
  delay(10);
  port->ODR &= ~mask;
  delay(10);

  // clear screen
  clear_lcd();
  
} // reset_lcd



/**
  \fn void clear_lcd(void)
   
  \brief clear LCD screen
  
  clear screen by filling with space
*/
void clear_lcd() {
  
  char str[20] = "                   ";  
  print_lcd_s(1, 1, str);
  print_lcd_s(2, 1, str);
  
} // clear_lcd



/**
  \fn uint8_t print_lcd_s(uint8_t line, uint8_t col, uint8_t *str)
   
  \brief print to LCD display
  
  \param  line    line to print to (1 or 2)
  \param  col     column to start at (1..16)
  \param  str     string to print

  \return operation successful?

  print string to 2x16 LCD display
  
*/
uint8_t print_lcd_s(uint8_t line, uint8_t col, uint8_t *str) {

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
  result = I2C_write_sn(ADDR_LCD, 0x00, s, i);


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
  result |= I2C_write_sn(ADDR_LCD, 0x40, s, i);
  
  // return status
  return(result);

} // print_lcd_s



/**
  \fn uint8_t print_lcd_d(uint8_t line, uint8_t col, int num)
   
  \brief print number to LCD display
  
  \param  line    line to print to (1 or 2)
  \param  col     column to start at (1..16)
  \param  num     number to print

  \return operation successful?

  print number to 2x16 LCD display
  
*/
uint8_t print_lcd_d(uint8_t line, uint8_t col, int num) {

  uint8_t  str[20], result;

  // convert number to string
  sprintf(str, "%ld", (long) num);
  
  // output to LCD
  result = print_lcd_s(line, col, str);
  
  // return status
  return(result);

} // print_lcd_d



void setup()
{
  // init LCD
  reset_lcd(GPIOE, 3);

  // print greeting
  print_lcd_s(1, 6, "hello");
  print_lcd_s(2, 6, "world");
  delay(2000);
  clear_lcd();
  
} // setup



void loop()
{
  clear_lcd();
  print_lcd_s(1, 1, "time: ");
  print_lcd_d(1, 7, millis()/1000L);
  print_lcd_s(2, 5, "seconds");
  delay(1000);
  
} // loop
