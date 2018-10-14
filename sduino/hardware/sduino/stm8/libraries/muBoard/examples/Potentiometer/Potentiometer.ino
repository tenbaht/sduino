/*
  muBoard Library - Hello World

 Simple project to show muBoard usage
 
 Read potentiometer and print via console.
 Can be displayed e.g. via via Serial Plotter.

 This example code is in the public domain.

*/

// include the library code:
#include <muBoard.h>


void setup() {
  
  // init communication to PC
  Serial_begin(115200);
  
} // setup()


void loop() {

  // measure voltage on poti
  uint16_t val = analogRead(POTI);
  
  // print poti reading to console
  Serial_println_i(val);

  
  // print to LCD
  lcd_clear();
  lcd_print_s(1, 1, "poti:");
  lcd_print_i(2, 1, val);

  // wait a bit
  delay(100);

} // loop()
