/*
 * Thermometer - read temperature using an LM35 sensor and display it on a PCD8544 LCD.
 *
 * Copyright (c) 2010 Carlos Rodrigues <cefrodrigues@gmail.com>
 * modified 2017 by Michael Mayer <michael-mayer@gmx.de>
 *   - ported to C for use with SDCC and sduino
 *   - added oversampling for the sensor reading
 *   - avoid float arithmetics by using fixed point arithmetics
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
 *
 * Connect the LM35 sensor output to the analog input 1 on PD2.
 */

#include <PCD8544.h>

/*
 * oversampling: read the analog input multiple times and take the average
 * value of all the readings for the actual value. The sensor is read
 * 2^OVERSAMPLING times. This can increase the resolution of the sensor
 * reading by OVERSAMPLING bits.
 */
#define OVERSAMPLING	4

static const byte sensorPin = A1;
static const byte ledPin = LED_BUILTIN;

// The dimensions of the LCD (in pixels)...
#define LCD_WIDTH	84
#define LCD_HEIGHT	48

// The number of lines for the temperature chart...
#define CHART_HEIGHT	5

// A custom "degrees" symbol...
#define DEGREES_CHAR	1
static const byte degrees_glyph[] = { 0x00, 0x07, 0x05, 0x07, 0x00 };

// A bitmap graphic (10x2) of a thermometer...
#define THERMO_WIDTH	10
#define THERMO_HEIGHT	2
static const byte thermometer[] = { 0x00, 0x00, 0x48, 0xfe, 0x01, 0xfe, 0x00, 0x02, 0x05, 0x02,
                                    0x00, 0x00, 0x62, 0xff, 0xfe, 0xff, 0x60, 0x00, 0x00, 0x00};

PCD8544 (lcd, PC5, PC6, PC4, PC7, PD1);	// sclk,sdin,dc,reset,sce


void setup() {
  lcd_begin_wh(LCD_WIDTH, LCD_HEIGHT);

  // Register the custom symbol...
  lcd_createChar(DEGREES_CHAR, degrees_glyph);

  pinMode(ledPin, OUTPUT);

  // Unfortunatly, there is no build in analog reference for the STM8S and
  // we have to use the full 3.3V supply voltage as a reference.
  //
  // If you are using a device with an internal reference voltage enable it
  // here and adjust the formula below for the temperature calculation 
  // accordingly.
//  analogReference(INTERNAL);
}


void loop() {
  // Start beyond the edge of the screen...
  static byte xChart = LCD_WIDTH;
  unsigned int temp = 0;
  unsigned char i;

  digitalWrite(ledPin, HIGH);

  // read the sensor multiple times
  for (i=(1<<OVERSAMPLING); i; i--) temp += analogRead(sensorPin);

  // Read the temperature (in celsius)...
  // The sensor delivers 10mV/°C, 0°C=0V, 100°C=1V
  // The STM8S does not offer an internal reference voltage and uses Vcc
  // (3.3V) instead.
  // temp = (analog/1023) * 3.3V / 10mV/°C
  //      = (analog/1023) * 330 [°C]
  //      = analog * 330 / 1023 [°C]
  // to avoid float arithmetics we can calculate in units of 0.1°C instead:
  // temp = analog * 3300 / 1023 [0.1°C]
  // without loosing too much precision we could divide by 1024 instead:
  // temp = analog * 3300 / 1024 = analog * 1650 / 512 = (analog*1650)>>9
  temp = ( (unsigned long) temp * 1650UL) >> (9+OVERSAMPLING);

  // Print the temperature (using the custom "degrees" symbol)...
  lcd_setCursor(0, 0);
  lcd_print_s("Temp: ");
  lcd_print_u(temp/10);
  lcd_print_c('.');
  lcd_print_u(temp%10);
  lcd_print_s(" \001C ");

  // Draw the thermometer bitmap at the bottom left corner...
  lcd_setCursor(0, LCD_HEIGHT/8 - THERMO_HEIGHT);
  lcd_drawBitmap(thermometer, THERMO_WIDTH, THERMO_HEIGHT);

  // Wrap the chart's current position...
  if (xChart >= LCD_WIDTH) {
    xChart = THERMO_WIDTH + 2;
  }

  // Update the temperature chart...  
  lcd_setCursor(xChart, 1);
  // ...clipped to the 0-45C range (still calulating with 0.1C units)
  lcd_drawColumn(CHART_HEIGHT, map(temp, 0, 450, 0, CHART_HEIGHT*8));
  lcd_drawColumn(CHART_HEIGHT, 0);         // ...with a clear marker to see the current chart position.

  xChart++;

  digitalWrite(ledPin, LOW);  
  delay(500);
}


/* EOF - Thermometer.ino  */

