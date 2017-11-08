/*
  LiquidCrystal Library - Autoscroll

 Demonstrates the use a 16x2 LCD display.  The LiquidCrystal
 library works with all LCD displays that are compatible with the
 Hitachi HD44780 driver. There are many of them out there, and you
 can usually tell them by the 16-pin interface.

 This sketch demonstrates the use of the autoscroll()
 and noAutoscroll() functions to make new text scroll or not.

 The circuit:
 * LCD RS pin to PA1 (digital pin 0)
 * LCD Enable pin to PA2 (digital pin 1)
 * LCD D4 pin to PA3 (digital pin 2)
 * LCD D5 pin to PD2 (digital pin 11)
 * LCD D6 pin to PD3 (digital pin 12)
 * LCD D7 pin to PD4 (digital pin 13)
 * LCD R/W pin to ground
 * LCD VSS pin to ground
 * LCD VCC pin to 5V
 * 10K potentiometer:
 * ends to +5V and ground
 * wiper to LCD VO pin (pin 3)

 Library originally added 18 Apr 2008
 by David A. Mellis
 library modified 5 Jul 2009
 by Limor Fried (http://www.ladyada.net)
 example added 9 Jul 2009
 by Tom Igoe
 modified 22 Nov 2010
 by Tom Igoe
 modified 23 Jan 2017
 by Michael Mayer

 This example code is in the public domain.

 http://www.arduino.cc/en/Tutorial/LiquidCrystalAutoscroll

 */

// include the library code:
#include <LiquidCrystal.h>

// initialize the library with the numbers of the interface pins
LiquidCrystal (lcd,PA1,PA2, PA3,PD2,PD3,PD4);

void setup() {
  // set up the LCD's number of columns and rows:
  lcd_begin(16, 2);
}

void loop() {
  // set the cursor to (0,0):
  lcd_setCursor(0, 0);
  // print from 0 to 9:
  for (int thisChar = 0; thisChar < 10; thisChar++) {
    lcd_print_u(thisChar);
    delay(500);
  }

  // set the cursor to (16,1):
  lcd_setCursor(16, 1);
  // set the display to automatically scroll:
  lcd_autoscroll();
  // print from 0 to 9:
  for (int thisChar = 0; thisChar < 10; thisChar++) {
    lcd_print_u(thisChar);
    delay(500);
  }
  // turn off automatic scrolling
  lcd_noAutoscroll();

  // clear screen for the next loop:
  lcd_clear();
}

