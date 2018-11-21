#include <I2C.h>
#include <LiquidCrystal_I2C.h>

// The PCF8574 exists in three variants using two different I2C address ranges:
//
// PCF8574 and PCF8574T (most common): 0x20..0x27, typ. 0x27
// PCF8574A: 0x38..0x3f, typ. 0x3f

// Set the LCD address to 0x27 for a 16 chars and 2 line display
#define LCD_ADDR 0x27

LiquidCrystal_I2C(lcd, 0x27, 16, 2);

void setup()
{
	// initialize the LCD
	lcd_begin();

	// Turn on the blacklight and print a message.
	lcd_backlight();
	lcd_print_s("Hello, world!");
}

void loop()
{
	lcd_setCursor(0, 1);
	lcd_print_u(millis() / 1000);
}
