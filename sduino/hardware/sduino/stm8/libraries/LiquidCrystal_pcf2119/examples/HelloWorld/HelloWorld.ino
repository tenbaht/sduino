#include <I2C.h>
#include <LiquidCrystal_pcf2119.h>

// Set the LCD address to 0x3c for a 16 chars and 2 line display

LiquidCrystal_pcf2119(lcd, 0x3c);

void setup()
{
	// initialize the LCD
	lcd_begin(16,2);

	// Print a message.
	lcd_print_s("Hello, world!");
}

void loop()
{
	// Do nothing here...
}
