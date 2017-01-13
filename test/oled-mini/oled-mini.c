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

#include "ssd1306.h"

#define OLED_RESET 6

#if (SSD1306_LCDHEIGHT != 64)
#error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif

void UART1_RX_IRQHandler(void) __interrupt(ITC_IRQ_UART1_RX){}
void UART1_TX_IRQHandler(void) __interrupt(ITC_IRQ_UART1_TX){}

void setup()   {                
//  Serial_begin(9600);

  Adafruit_SSD1306_Adafruit_SSD1306(OLED_RESET);

//  display_init(OLED_RESET);
  // by default, we'll generate the high voltage from the 3.3v line internally! (neat!)
  Adafruit_SSD1306_begin(SSD1306_SWITCHCAPVCC, 0x3C,1);  // initialize with the I2C addr 0x3D (for the 128x64)
  // init done
  
  // Show image buffer on the display hardware.
  // Since the buffer is intialized with an Adafruit splashscreen
  // internally, this will display the splashscreen.
//  Adafruit_SSD1306_display();
}


void loop()
{
	uint8_t x,y;

	Adafruit_SSD1306_display();
	delay (100);
	for (x=0; x<WIDTH; x+=8)
	{
		for (y=0; y<HEIGHT; y+=8)
		{
			drawPixel(x,y,INVERSE);
		}
	}
}
