/*
 * PCD8544 - Interface with Philips PCD8544 (or compatible) LCDs.
 *
 * Copyright (c) 2010 Carlos Rodrigues <cefrodrigues@gmail.com>
 * Ported to C for use with SDCC 2017 Michael Mayer <michael-mayer@gmx.de>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

/*
 * To use this sketch with a STM8S103 breakout board, connect the eight pins
 * from your 5110 LCD like this:
 *
 * Pin 1 (VCC)  -> +3.3V (marked by a square around the pin on the silkscreen)
 * Pin 2 (GND)  -> Ground
 * Pin 3 (SCE)  -> STM8S103 pin PD1 (sduino digital pin 10)
 * Pin 4 (RST)  -> STM8S103 pin PC7 (sduino digital pin 9)
 * Pin 5 (D/C)  -> STM8S103 pin PC4 (sduino digital pin 6)
 * Pin 6 (MOSI) -> STM8S103 pin PC6 (sduino digital pin 8)
 * Pin 7 (SCLK) -> STM8S103 pin PC5 (sduino digital pin 7)
 * Pin 8 (LED)  -> 330 ohm resistor to Vcc (3.3V) (or to a PWM pin)
 *
 * Since the STM8S works on 3.3V you can connect the data lines directly to
 * port lines. If you are using this display with a 5V CPU (like on most
 * Arduino boards), you have to add extra components to connect it to the
 * digital pins of the Arduino (not necessary if you are using a 3.3V
 * variant of the Arduino, such as Sparkfun's Arduino Pro).
 */

#include <PCD8544.h>


// A custom glyph (a smiley)...
static const byte glyph[] = { B00010000, B00110100, B00110000, B00110100, B00010000 };


PCD8544 (lcd, PC5, PC6, PC4, PC7, PD1);	// sclk,sdin,dc,reset,sce


void setup() {
  // PCD8544-compatible displays may have a different resolution...
  lcd_begin_wh(84, 48);

  // Add the smiley to position "0" of the ASCII table...
  lcd_createChar(0, glyph);
}


void loop() {
  // Just to show the program is alive...
  static int counter = 0;

  // Write a piece of text on the first line...
  lcd_setCursor(0, 0);
  lcd_print_s("Hello, World!");

  // Write the counter on the second line...
  lcd_setCursor(0, 1);
  lcd_print_u(counter);
  lcd_write(' ');
  lcd_write(0);  // write the smiley

  // Use a potentiometer to set the LCD contrast...
  // short level = map(analogRead(A1), 0, 1023, 0, 127);
  // lcd_setContrast(level);

  delay(200);
  counter++;
}


/* EOF - HelloWorld.ino */
