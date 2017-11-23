/*
 * This is an example for using the i2c library with a monochrome display
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
 * modified 2017 by Paul Smith
 */

/*********************************************************************
This is an example for our Monochrome displays based on SSD1306 drivers

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

#define display_RESET -1

Mini_SSD1306(display, display_RESET);

#if (SSD1306_LCDHEIGHT != 64)
#error("Height incorrect, please fix Mini_SSD1306.h!");
#endif

#define BME280 0x76
#define BUFFER_SIZE 8
static const char *helloWorld = "HelloWorld!";
static const char *foundBME280 = "Found BME280";

// start at first printable char 0x20, see FONT_5x7.h
uint8_t count = 0x20;

void setup()
{
	I2C_begin();

	// The Nokia 5110 display generates the high voltage from the 3.3v
	// line internally! (neat!)
	// Initialize with the I2C addr 0x3C. Some displays use 0x3D instead.
	display_begin(SSD1306_SWITCHCAPVCC, 0x3C, 0);
	// initially clear the screen
	display_clearPages();

	I2C_write_c(BME280, 0xF2, 0x01); // hum oversampling 1
	I2C_write_c(BME280, 0xF4, 0x27); // temp/pres oversampling 1
	I2C_write_c(BME280, 0xF5, 0xA0); // 1Hz
}

void loop()
{
	uint8_t buffer[BUFFER_SIZE];
	uint8_t res;

	delay(1000);
	// only redraw part of the screen
	display_setCursor(0, 0);
	display_printString(helloWorld);
	// printChar is hardcoded to avoid non printable chars
	display_printChar(count);
	count++;

	// write to bme280 (0x76), set register to id (0xD0)
	I2C_write(BME280, 0xD0);
	res = I2C_read(BME280, &buffer[0], 1);

	if (buffer[0] == 0x60)
	{
		display_setCursor(0, 1);
		display_printString(foundBME280);

		display_setCursor(0, 2);
		I2C_write(BME280, 0xF7); // start of data and burst
		res = I2C_read(BME280, &buffer[0], BUFFER_SIZE);
		display_sendByte(res); // debug
		display_sendByte(res); // debug
		display_sendByte(res); // debug

		display_sendByte(0x00); // debug
		display_sendByte(0x00); // debug
		display_sendByte(0x00); // debug

		for (uint8_t i = 0; i < BUFFER_SIZE; i++)
		{
			display_sendByte(buffer[i]);
			display_sendByte(buffer[i]);
			display_sendByte(buffer[i]);
		}
	}
}