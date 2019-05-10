#include "LiquidCrystal_pcf2119.h"

//#include <stdio.h>
//#include <string.h>
//#include <inttypes.h>
#include "Arduino.h"
#ifdef USE_WIRE
  #include "Wire.h"
#else
  #include "I2C.h"
#endif

/*
 * This library is for I2C connected text mode LCDs based on the PCF2119 controller
 *
 * Default I2C address 0x3c, alternative address 0x3d
 *
 */
// When the display powers up, it is configured as follows:
//
// 1. Display clear
// 2. Function set:
//    DL = 1; 8-bit interface data
//    N = 0; 1-line display
//    F = 0; 5x8 dot character font
// 3. Display on/off control:
//    D = 0; Display off
//    C = 0; Cursor off
//    B = 0; Blinking off
// 4. Entry mode set:
//    I/D = 1; Increment by 1
//    S = 0; No shift
//
// Note, however, that resetting the Arduino doesn't reset the LCD, so we
// can't assume that its in that state when a sketch starts (and the
// LiquidCrystal constructor is called).

// private constants

#define CHARSET_ASCII		0
#define CHARSET_FLIP		1
#define CHARSET_FLIP_SPACE	2


// private variables

static void LiquidCrystal_pcf2119_send(uint8_t value, uint8_t mode);

static uint8_t _displayfunction;
static uint8_t _displaycontrol;
static uint8_t _displaymode;

static uint8_t _addr;	// I2C address
static uint8_t _cols;
static uint8_t _rows;
static uint8_t _rstPin;
static uint8_t _charsize;
static uint8_t _row_offsets[4];
static uint8_t _charset;	// type of charset: ASCII, FLIP, FLIP_SPACE


// private functions

static void LiquidCrystal_pcf2119_charset(char charset);


// public functions/methods

void LiquidCrystal_pcf2119_init(uint8_t lcd_addr, uint8_t lcd_rst, char charset)
{
	_addr = lcd_addr;
	_rstPin = lcd_rst;
	LiquidCrystal_pcf2119_charset(charset);
}


void LiquidCrystal_pcf2119_begin(uint8_t lcd_cols, uint8_t lcd_rows, uint8_t charsize)
{
	_cols = lcd_cols;
	_rows = lcd_rows;
	_charsize = charsize;

	if (_rstPin != 255)
	{
		// do a hardware reset if reset pin is defined
		pinMode(_rstPin, OUTPUT);
		digitalWrite(_rstPin, LOW);
		delay(10);
		digitalWrite(_rstPin, HIGH);
		delay(100);
		digitalWrite(_rstPin, LOW);
	}

#ifdef USE_WIRE
	Wire_begin();
#else
	I2C_begin();
#endif
	_displayfunction = LCD_8BITMODE | LCD_1LINE | LCD_5x8DOTS;

	if (_rows > 1) {
		_displayfunction |= LCD_2LINE;
	}
	_row_offsets[0] = 0;
	_row_offsets[1] = 0x40;
	_row_offsets[2] = _cols;
	_row_offsets[3] = 0x40+_cols;

	// for some 1 line displays you can select a 10 pixel high font
	if ((_charsize != 0) && (_rows == 1)) {
		_displayfunction |= LCD_5x10DOTS;
	}


	// SEE PAGE 64 FOR INITIALIZATION SPECIFICATION!
	// according to datasheet, we need at least 40ms after power rises above 2.7V
	// before sending commands. Arduino can turn on way before 4.5V so we'll wait 50
	delay(50);


	// Send function set command sequence
	LiquidCrystal_pcf2119_command(LCD_FUNCTIONSET | _displayfunction);
	delayMicroseconds(4500);  // wait more than 4.1ms

/* not sure if these are needed, but keeping it for reference
	// second try
	LiquidCrystal_pcf2119_command(LCD_FUNCTIONSET | _displayfunction);
	delayMicroseconds(150);

	// third go
	LiquidCrystal_pcf2119_command(LCD_FUNCTIONSET | _displayfunction);

	// finally, set # lines, font size, etc.
	LiquidCrystal_pcf2119_command(LCD_FUNCTIONSET | _displayfunction);
*/
	
	// turn the display on with no cursor or blinking default
	_displaycontrol = LCD_DISPLAYON | LCD_CURSOROFF | LCD_BLINKOFF;
	LiquidCrystal_pcf2119_display();

	// clear it off
	LiquidCrystal_pcf2119_clear();

	// Initialize to default text direction (for roman languages)
	_displaymode = LCD_ENTRYLEFT | LCD_ENTRYSHIFTDECREMENT;
	// set the entry mode
	LiquidCrystal_pcf2119_command(LCD_ENTRYMODESET | _displaymode);

	LiquidCrystal_pcf2119_home();
}

/********** high level commands, for the user! */
void LiquidCrystal_pcf2119_clear()
{
	if (_charset == CHARSET_FLIP_SPACE) {
		// non-ASCII charset R -> manually fill with space
		// see page 32 in datasheet

		LiquidCrystal_pcf2119_home();

		uint8_t i=_cols*_rows;
		while (i--) {
			LiquidCrystal_pcf2119_data(' '|0x80);
		}

		LiquidCrystal_pcf2119_home();
	} else {
		// ASCII charset (A,D,F,I,S) -> use built-in clear

		LiquidCrystal_pcf2119_command(LCD_CLEARDISPLAY);  // clear display, set cursor position to zero
		delayMicroseconds(2000);  // this command takes a long time!
	}
}

void LiquidCrystal_pcf2119_home()
{
  LiquidCrystal_pcf2119_command(LCD_RETURNHOME);  // set cursor position to zero
  delayMicroseconds(2000);  // this command takes a long time!
}

void LiquidCrystal_pcf2119_setCursor(uint8_t col, uint8_t row)
{
  // clip to display size, starting w/0
  if ( col >= _cols ) col = _cols - 1;
  if ( row >= _rows ) row = _rows - 1;

  LiquidCrystal_pcf2119_command(LCD_SETDDRAMADDR + row*_cols + col);
}

// Turn the display on/off (quickly)
void LiquidCrystal_pcf2119_noDisplay() {
  _displaycontrol &= ~LCD_DISPLAYON;
  LiquidCrystal_pcf2119_command(LCD_DISPLAYCONTROL | _displaycontrol);
}
void LiquidCrystal_pcf2119_display() {
  _displaycontrol |= LCD_DISPLAYON;
  LiquidCrystal_pcf2119_command(LCD_DISPLAYCONTROL | _displaycontrol);
}

// Turns the underline cursor on/off
void LiquidCrystal_pcf2119_noCursor() {
  _displaycontrol &= ~LCD_CURSORON;
  LiquidCrystal_pcf2119_command(LCD_DISPLAYCONTROL | _displaycontrol);
}
void LiquidCrystal_pcf2119_cursor() {
  _displaycontrol |= LCD_CURSORON;
  LiquidCrystal_pcf2119_command(LCD_DISPLAYCONTROL | _displaycontrol);
}

// Turn on and off the blinking cursor
void LiquidCrystal_pcf2119_noBlink() {
  _displaycontrol &= ~LCD_BLINKON;
  LiquidCrystal_pcf2119_command(LCD_DISPLAYCONTROL | _displaycontrol);
}
void LiquidCrystal_pcf2119_blink() {
  _displaycontrol |= LCD_BLINKON;
  LiquidCrystal_pcf2119_command(LCD_DISPLAYCONTROL | _displaycontrol);
}

// These commands scroll the display without changing the RAM
void LiquidCrystal_pcf2119_scrollDisplayLeft(void) {
  LiquidCrystal_pcf2119_command(LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVELEFT);
}
void LiquidCrystal_pcf2119_scrollDisplayRight(void) {
  LiquidCrystal_pcf2119_command(LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVERIGHT);
}

// This is for text that flows Left to Right
void LiquidCrystal_pcf2119_leftToRight(void) {
  _displaymode |= LCD_ENTRYLEFT;
  LiquidCrystal_pcf2119_command(LCD_ENTRYMODESET | _displaymode);
}

// This is for text that flows Right to Left
void LiquidCrystal_pcf2119_rightToLeft(void) {
  _displaymode &= ~LCD_ENTRYLEFT;
  LiquidCrystal_pcf2119_command(LCD_ENTRYMODESET | _displaymode);
}

// This will 'right justify' text from the cursor
void LiquidCrystal_pcf2119_autoscroll(void) {
  _displaymode |= LCD_ENTRYSHIFTINCREMENT;
  LiquidCrystal_pcf2119_command(LCD_ENTRYMODESET | _displaymode);
}

// This will 'left justify' text from the cursor
void LiquidCrystal_pcf2119_noAutoscroll(void) {
  _displaymode &= ~LCD_ENTRYSHIFTINCREMENT;
  LiquidCrystal_pcf2119_command(LCD_ENTRYMODESET | _displaymode);
}

// Allows us to fill the first 8 CGRAM locations
// with custom characters
void LiquidCrystal_pcf2119_createChar(uint8_t location, uint8_t charmap[]) {
  location &= 0x7; // we only have 8 locations 0-7
  LiquidCrystal_pcf2119_command(LCD_SETCGRAMADDR | (location << 3));
  for (int i=0; i<8; i++) {
    LiquidCrystal_pcf2119_data(charmap[i]);
  }
  // switch back to character output mode
  LiquidCrystal_pcf2119_command(LCD_SETDDRAMADDR);
}


/*********** internal helper functions */

// Set character set (default=ASCII). Some charsets require remapping,
// see page 19ff in https://www.nxp.com/docs/en/data-sheet/PCF2119X.pdf

/* The PCF2119 comes in six versions with different character sets.
 * Unfortunatly, not all of them use a ASCII compatible encoding for
 * at least the latin alphabet. Some of them encode the latin characters
 * in the higher half of the codepage (bit 7 set).
 * One of them (charset R) even encodes the space character differently,
 * rendering the build in clear command pointless and requiring a manual
 * memory sweep to clear the display content.
 *
 * A: standard ASCII charset, similar to HD4480 controller
 * D: ASCII, but upper case only
 * F: non-ASCII. Bit 7 needs to be flipped.
 * I: ASCII
 * R: non-ASCII. Bit 7 needs to be flipped, even space is at 0xa0 instead of 0x20
 * S: non-ASCII. Bit 7 needs to be flipped.
 */

static void LiquidCrystal_pcf2119_charset(char charset) {

	charset &= ~0x20;	// convert lower to uppper case
	if ((charset=='F') || (charset=='S'))
	{
		_charset = CHARSET_FLIP;
	}
	else if (charset=='R')
	{
		_charset = CHARSET_FLIP_SPACE;
	}
	else
	{
		_charset=CHARSET_ASCII;
	}
} // LiquidCrystal_pcf2119_charset()


/*********** mid level commands, for sending data/cmds */

void LiquidCrystal_pcf2119_command(uint8_t value) {
	// write to command register
	I2C_write_reg(_addr, 0, value);
}

// write a data byte.
size_t LiquidCrystal_pcf2119_data(uint8_t value) {
	// write to data register
	// return 1 for success (if I2C_write_reg returned 0)
	return (
		I2C_write_reg(_addr, 0x40, value) ? 0 : 1
	);
}

// write a character. Do ASCII conversion if needed
size_t LiquidCrystal_pcf2119_write(uint8_t value) {
	if (_charset != CHARSET_ASCII) {
		// do not convert values 0x00..0x1f/0x80..0x9f
		// this would mess up the user defined characters
		if ((value&0x7f) >= 32) {
			value ^= 0x80;
		}
	}
	return LiquidCrystal_pcf2119_data(value);
}
/************ low level data pushing commands **********/
// there is no need for another level of indirection for direct I2C access
