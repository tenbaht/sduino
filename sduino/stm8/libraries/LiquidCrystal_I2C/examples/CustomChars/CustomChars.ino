/**
 * Shows off how to use custom characters
 *
 * modified for use with SDuino
 * 25.10.2018 by Michael Mayer
 */
#include <I2C.h>
#include <LiquidCrystal_I2C.h>

uint8_t bell[8]  = {0x4, 0xe, 0xe, 0xe, 0x1f, 0x0, 0x4};
uint8_t note[8]  = {0x2, 0x3, 0x2, 0xe, 0x1e, 0xc, 0x0};
uint8_t clock[8] = {0x0, 0xe, 0x15, 0x17, 0x11, 0xe, 0x0};
uint8_t heart[8] = {0x0, 0xa, 0x1f, 0x1f, 0xe, 0x4, 0x0};
uint8_t duck[8]  = {0x0, 0xc, 0x1d, 0xf, 0xf, 0x6, 0x0};
uint8_t check[8] = {0x0, 0x1 ,0x3, 0x16, 0x1c, 0x8, 0x0};
uint8_t cross[8] = {0x0, 0x1b, 0xe, 0x4, 0xe, 0x1b, 0x0};
uint8_t retarrow[8] = {	0x1, 0x1, 0x5, 0x9, 0x1f, 0x8, 0x4};

// Set the LCD address to 0x27 for a 16 chars and 2 line display
LiquidCrystal_I2C (lcd,0x27, 16, 2);

void setup()
{
	lcd_begin();
	lcd_backlight();

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

