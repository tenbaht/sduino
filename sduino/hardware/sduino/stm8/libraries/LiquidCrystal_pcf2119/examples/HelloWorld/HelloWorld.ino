#include <I2C.h>
#include <LiquidCrystal_pcf2119.h>

#define LCD_ADDR  0x3B    // I2C address: 0x3A or 0x3B, dep. on pin SA0 state
#define LCD_RST   PE3     // LCD reset pin (active high)

// Set LCD address and reset pin
LiquidCrystal_pcf2119(lcd, LCD_ADDR, LCD_RST);

void setup()
{  
  char  str[32];  // required for non-ASCII character set (R,S)
  
  // initialize the LCD (16x2 display)
	lcd_begin(16,2);

  // optionally set non-ASCII character set (type R+S)
  lcd_charset(NON_ASCII);
  
	// Print a message.
	sprintf(str, "Hello world!");
	lcd_convert_s(str);
	lcd_print_s(str);

  for (int i; i<5; i++) {
    lcd_noDisplay();
    delay(500);
    lcd_display();
    delay(500);
  }

  // doesn't work for non-ASCII: fills left with non-space char
  for (int i; i<16*2; i++) {
    lcd_scrollDisplayRight();
    delay(200);
  }
    
  
}

void loop()
{
	// Do nothing here...
}
