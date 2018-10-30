/**
 * Demonstrates the use of lcd_Blink() and lcd_noBlink()
 *
 * modified for use with SDuino
 * 30.10.2018 by Michael Mayer
 */
#include <I2C.h> 
#include <LiquidCrystal_pcf2119.h>

LiquidCrystal_pcf2119 (lcd,0x3c);

void setup()
{
	// initialize the LCD
	lcd_begin(16,2);
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
