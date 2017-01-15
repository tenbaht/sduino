/*
 * This is an example for using the i2c library with a monochrome OLED
 * display based on SSD1306 drivers.
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

#include "I2C.h"
#include "ssd1306.h"

#define OLED_RESET -1

#if (SSD1306_LCDHEIGHT != 64)
#error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif

// empty IRQ handler just to keep the linker happy as we are not using the
// Hardware_Serial library
void UART1_RX_IRQHandler(void) __interrupt(ITC_IRQ_UART1_RX){}
void UART1_TX_IRQHandler(void) __interrupt(ITC_IRQ_UART1_TX){}


void setup()
{
	display_init(OLED_RESET);

	// by default, we'll generate the high voltage from the 3.3v line internally! (neat!)
	display_begin(SSD1306_SWITCHCAPVCC, 0x3C,0);  // initialize with the I2C addr 0x3C
	// init done
  
	// Show image buffer on the display hardware.
	// Since the buffer is intialized with an Adafruit splashscreen
	// internally, this will display the splashscreen.
	// display_display();
}


void loop()
{
	uint8_t x,y;

	display_display();	// show the splash screen
	//display_startscrollright(4,6);

	delay (1000);

	// draw some dots in an 8x8 pattern
	for (x=0; x<WIDTH; x+=8)
	{
//		for (y=0; y<HEIGHT; y+=8)
		for (y=0; y<56; y+=8)	// don't mess with the stack!
		{
			drawPixel(x,y,INVERSE);
		}
	}
}
