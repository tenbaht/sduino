/**
 * Shows off how to use custom characters
 *
 * modified for use with SDuino
 * 30.10.2018 by Michael Mayer
 */
#include <I2C.h>
#include <LiquidCrystal_pcf2119.h>

uint8_t bell[8]  = {0x4, 0xe, 0xe, 0xe, 0x1f, 0x0, 0x4};
uint8_t note[8]  = {0x2, 0x3, 0x2, 0xe, 0x1e, 0xc, 0x0};
uint8_t clock[8] = {0x0, 0xe, 0x15, 0x17, 0x11, 0xe, 0x0};
uint8_t heart[8] = {0x0, 0xa, 0x1f, 0x1f, 0xe, 0x4, 0x0};
uint8_t duck[8]  = {0x0, 0xc, 0x1d, 0xf, 0xf, 0x6, 0x0};
uint8_t check[8] = {0x0, 0x1 ,0x3, 0x16, 0x1c, 0x8, 0x0};
uint8_t cross[8] = {0x0, 0x1b, 0xe, 0x4, 0xe, 0x1b, 0x0};
uint8_t retarrow[8] = {	0x1, 0x1, 0x5, 0x9, 0x1f, 0x8, 0x4};

// LCD controller settings
#define LCD_ADDR    0x3B    // I2C address: 0x3A or 0x3B, dep. on pin SA0 state
#define LCD_RST     PE3     // reset pin (active high)
#define LCD_CHARSET 'R'     // character set (A, D, F, I, R or S)

// Set LCD address and reset pin
LiquidCrystal_pcf2119(lcd, LCD_ADDR, LCD_RST, LCD_CHARSET);

void displayKeyCodes(void);

void setup()
{
	// initialize the LCD (16x2 display)
	lcd_begin(16,2);

	// create special characters
	lcd_createChar(0, bell);
	lcd_createChar(1, note);
	lcd_createChar(2, clock);
	lcd_createChar(3, heart);
	lcd_createChar(4, duck);
	lcd_createChar(5, check);
	lcd_createChar(6, cross);
	lcd_createChar(7, retarrow);
	lcd_home();

	lcd_print_s("Hello world...");
	lcd_setCursor(0, 1);
	lcd_print_s(" i ");
	lcd_write(3);
	lcd_print_s(" arduinos!");
	delay(5000);
	displayKeyCodes();
}

// display all keycodes
void displayKeyCodes(void) {
	uint8_t i = 0;

	while (1) {
		lcd_clear();
		lcd_print_s("Codes 0x");
		lcd_print_ub(i, HEX);
		lcd_print_s("-0x");
		lcd_print_ub(i + 16, HEX);
		lcd_setCursor(0, 1);

		for (int j = 0; j < 16; j++) {
			lcd_write(i + j);
		}
		i += 16;

		delay(4000);
	}
}

void loop()
{
	// Do nothing here...
}
