/**
 * Displays text sent over the serial port (e.g. from the Serial Monitor) on
 * an attached LCD.
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
	lcd_begin();
	lcd_backlight();
  
	// Initialize the serial port at a speed of 9600 baud
	Serial_begin(9600);
}

void loop()
{
	// If characters arrived over the serial port...
	if (Serial_available()) {
		// Wait a bit for the entire message to arrive
		delay(100);
		// Clear the screen
		lcd_clear();

		// Write all characters received with the serial port to the LCD.
		while (Serial_available() > 0) {
			lcd_write(Serial_read());
		}
	}
}
