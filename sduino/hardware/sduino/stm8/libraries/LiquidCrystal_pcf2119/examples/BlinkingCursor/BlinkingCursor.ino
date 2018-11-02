/**
 * Demonstrates the use of lcd_Blink() and lcd_noBlink()
 *
 * modified for use with SDuino
 * 30.10.2018 by Michael Mayer
 */
#include <I2C.h> 
#include <LiquidCrystal_pcf2119.h>

#define LCD_ADDR  0x3B    // I2C address: 0x3A or 0x3B, dep. on pin SA0 state
#define LCD_RST   PE3     // LCD reset pin (active high)

// Set LCD address and reset pin
LiquidCrystal_pcf2119(lcd, LCD_ADDR, LCD_RST);

void setup()
{
	// initialize the LCD (16x2 display)
  lcd_begin(16,2);

  // optionally set non-ASCII character set (type R+S)
  lcd_charset(NON_ASCII);
}

void loop()
{
  char  str[32];  // required for non-ASCII character set (R,S)
  bool blinking = true;
	lcd_cursor();

	while (1) {
		if (blinking) {
			lcd_clear();
      sprintf(str, "No cursor blink");
      lcd_convert_s(str);
      lcd_print_s(str);
			lcd_noBlink();
			blinking = false;
		} else {
			lcd_clear();
      sprintf(str, "Cursor blink");
      lcd_convert_s(str);
      lcd_print_s(str);
			lcd_blink();
			blinking = true;
		}
		delay(4000);
	}
}
