/*
  muBoard Library - LCD display
  
 Simple project to show muBoard usage
 
 Print to LCD

 This example code is in the public domain.

*/
 
// include the library code:
#include <muBoard.h>

// for scheduler
uint32_t  lastCall = 0L;


void setup() {

  // print greeting
  lcd_print_s(1, 6, "hello");
  lcd_print_s(2, 6, "world");
  delay(2000);
  lcd_clear();

} // setup()


void loop() {

  lcd_clear();
  lcd_print_s(1, 1, "time: ");
  lcd_print_d(1, 7, millis()/1000L);
  lcd_print_s(2, 5, "seconds");
  delay(1000);

} // loop()
