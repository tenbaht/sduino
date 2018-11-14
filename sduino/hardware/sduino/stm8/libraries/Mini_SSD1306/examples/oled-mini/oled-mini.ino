/*
 * This is an example for using the i2c library with a monochrome OLED
 * display based on SSD1306 drivers.
 *
 * It shows the Adafruit splash screen and blinks a pixel pattern once per
 * second. On CPUs with only 1kB RAM the display buffer overlaps with the
 * stack and you will see it as a wild pattern in the lower left corner of
 * the display. For this reason the blinking loop does not touch the lower
 * 8 rows of the display memory buffer.
 *
 * The display has 128x64 pixel and uses only SCL and SDA for communication,
 * there is no reset pin.
 *
 * The framebuffer needs to be kept in RAM as reading the display is not
 * supported by the driver chips. Since the STM8S103F3 has only 1kB RAM
 * total, we will see the stack contents in the lower part of the display
 * as a wild bit pattern. Using drawPixel() on this memory would mess up
 * the stack contents and would result in an immediate crash. So don't
 * use the lower lines on low memory devices!
 *
 * This code is adopted from the Adafruit example code contained in the
 * Adafruit_SSD1306 library.
 *
 * modified 2017 by Michael Mayer
 */

/*********************************************************************
This is an example for our Monochrome OLEDs based on SSD1306 drivers

  Pick one up today in the adafruit shop!
  ------> http://www.adafruit.com/category/63_98

This example is for a 128x64 size display using I2C to communicate
3 pins are required to interface (2 I2C and one reset)

Adafruit invests time and resources providing this open source code, 
please support Adafruit and open-source hardware by purchasing 
products from Adafruit!

Written by Limor Fried/Ladyada  for Adafruit Industries.  
BSD license, check license.txt for more information
All text above, and the splash screen must be included in any redistribution
*********************************************************************/


// I2C is included here to let the build system automatically be aware of this
// second-level dependency. Otherwise you would need to list it in the Makefile:
// ARDUINO_LIBS = I2C
#include "I2C.h"
#include "Mini_SSD1306.h"

#define OLED_RESET -1

Mini_SSD1306(display,OLED_RESET);

#if (SSD1306_LCDHEIGHT != 64)
#error("Height incorrect, please fix Mini_SSD1306.h!");
#endif


void setup()
{
	// The Nokia 5110 display generates the high voltage from the 3.3v
	// line internally! (neat!)
	// Initialize with the I2C addr 0x3C. Some displays use 0x3D instead.
	display_begin(SSD1306_SWITCHCAPVCC, 0x3C,0);
}


void loop()
{
	uint8_t x,y;

	// Show image buffer on the display hardware.
	// Since the buffer is intialized with an Adafruit splashscreen
	// internally, this will display the splashscreen.
	display_display();

	delay (1000);

	// draw some dots in an 8x8 pattern
	for (x=0; x<WIDTH; x+=8)
	{
		for (y=0; y<HEIGHT-16; y+=8)	// don't alter the stack!
		{
			display_drawPixel(x,y,INVERSE);
		}
	}
}
