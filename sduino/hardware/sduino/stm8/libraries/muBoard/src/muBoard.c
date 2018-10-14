#include <stdio.h>
#include <string.h>
//#include <inttypes.h>
#include "Arduino.h"
#include "muBoard.h"


void initVariant() {

  // configure GPIOs
  gpioInit();

  // init LCD
  lcd_begin(LCD_I2C_ADDR, LCD_RST_PORT, LCD_RST_PIN);

  // init beeper
  BEEP_Init(BEEP_FREQUENCY_2KHZ);
  BEEP_Cmd(DISABLE);

} // initVariant
