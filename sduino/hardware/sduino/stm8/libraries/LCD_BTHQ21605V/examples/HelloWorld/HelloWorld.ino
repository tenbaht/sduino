/*******************************************
 Sample sketch that prints text to a 2x16 LCD
 (Batron BTHQ21605V-COG-FSRE-I2C; Farnell: 1220409)
 via I2C.
 Configuration
   - connect SCL/SDA pins (with pull-ups)
   - connect LCD POR pin to STM8 PE3 (see lcd_reset())
   - set address to 0x3B (see define)
 *******************************************/

#include <LCD_BTHQ21605V.h>

#define ADDR    0x3B    // I2C address of LCD


void setup()
{
  // init LCD
  lcd_begin(ADDR, GPIOE, 3);

  // print greeting
  lcd_print_s(1, 6, "hello");
  lcd_print_s(2, 6, "world");
  delay(2000);
  lcd_clear();
  
} // setup


void loop()
{
  lcd_clear();
  lcd_print_s(1, 1, "time: ");
  lcd_print_i(1, 7, millis()/1000L);
  lcd_print_s(2, 5, "seconds");
  delay(1000);
  
} // loop
