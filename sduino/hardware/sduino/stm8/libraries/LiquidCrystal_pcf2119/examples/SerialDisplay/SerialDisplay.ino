/**
 * Displays text sent over the serial port (e.g. from the Serial Monitor) on
 * an attached LCD.
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
