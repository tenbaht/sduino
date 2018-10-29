/**
 * Demonstrates the use of lcd_Blink() and lcd_noBlink()
 *
 * modified for use with SDuino
 * 25.10.2018 by Michael Mayer
 */
#include <I2C.h> 
#include <LiquidCrystal_I2C.h>

// Set the LCD address to 0x27 for a 16 chars and 2 line display
LiquidCrystal_I2C (lcd,0x27, 16, 2);

void setup()
{
	// initialize the LCD
	lcd_begin();
}

void loop()
{
	bool blinking = true;
	lcd_cursor();

	while (1) {
		if (blinking) {
			lcd_clear();
			lcd_print_s("No cursor blink");
			lcd_noBlink();
			blinking = false;
		} else {
			lcd_clear();
			lcd_print_s("Cursor blink");
			lcd_blink();
			blinking = true;
		}
		delay(4000);
	}
}
