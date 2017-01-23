/*
  LiquidCrystal Library - Hello World

 Demonstrates the use a 16x2 LCD display.  The LiquidCrystal
 library works with all LCD displays that are compatible with the
 Hitachi HD44780 driver. There are many of them out there, and you
 can usually tell them by the 16-pin interface.

 This sketch prints "Hello World!" to the LCD
 and shows the time.

  The circuit:
  lcd_connection_4bit_r(PA2,PA3, PD2,PD3,PD6,PA1);
 * LCD RS pin to PA2 (digital pin 1)
 * LCD Enable pin to PA3 (digital pin 2)
 * LCD D4 pin to PD2 (digital pin 11)
 * LCD D5 pin to PD3 (digital pin 12)
 * LCD D6 pin to PD6 (digital pin 15)
 * LCD D7 pin to PA1 (digital pin 0)
 * LCD R/W pin to ground
 * LCD VSS pin to ground
 * LCD VCC pin to 5V
 * 10K resistor:
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

 http://www.arduino.cc/en/Tutorial/LiquidCrystal
 */

// include the library code:
#include <Arduino.h>
#include <LiquidCrystal.h>

void lcd_print(char *str)
{
    while (*str) lcd_write(*str++);
}

// a very crude conversion routine from uint16 to an ASCII string
void lcd_print_i(uint16_t n)
{
    char string[6];	// max. 5 digits for 16 bit integer
    char *buf;

    buf = string+5;
    *buf = 0;
    // calculate the digits starting with the LSD
    do {
        *--buf = (n%10)+'0';
        n = n/10;
    } while (n);

    // now output them in reverse order, starting with the MSD
    lcd_print(buf);
}


void setup() {
  // initialize the library with the numbers of the interface pins
  lcd_connection_4bit_r(PA2,PA3, PD2,PD3,PD6,PA1);
  // set up the LCD's number of columns and rows:
  lcd_begin(16, 2);
  // Print a message to the LCD.
  lcd_print("hello, world!");
}


void loop() {
  // set the cursor to column 0, line 1
  // (note: line 1 is the second row, since counting begins with 0):
  lcd_setCursor(0, 1);
  // print the number of seconds since reset:
  lcd_print_i(millis() / 1000);
}

