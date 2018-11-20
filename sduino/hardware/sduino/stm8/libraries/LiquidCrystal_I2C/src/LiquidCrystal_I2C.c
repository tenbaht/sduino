#include "LiquidCrystal_I2C.h"

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
 * This library is for HD44780 based text mode LCDs connected via a
 * PCF8575 i2c adapter.
 *
 * Default I2C address for most modules is 0x27

 P0	RS
 P1	RW
 P2	EN
 P3	Backlight enable
 P4	DB4
 P5	DB5
 P6	DB6
 P7	DB7
 
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

// private variables

static void LiquidCrystal_I2C_send(uint8_t value, uint8_t mode);
static void LiquidCrystal_I2C_write4bits(uint8_t value);
//static void LiquidCrystal_write8bits(uint8_t);
static void LiquidCrystal_I2C_expanderWrite(uint8_t _data);
static void LiquidCrystal_I2C_pulseEnable(uint8_t _data);

//static uint8_t _rs_pin; // LOW: command.  HIGH: character.
//static uint8_t _rw_pin; // LOW: write to LCD.  HIGH: read from LCD.
//static uint8_t _enable_pin; // activated by a HIGH pulse.
//static uint8_t _data_pins[8];

static uint8_t _displayfunction;
static uint8_t _displaycontrol;
static uint8_t _displaymode;

//static uint8_t _initialized;

static uint8_t _addr;	// I2C address
static uint8_t _cols;
static uint8_t _rows;	// was _numlines;
static uint8_t _charsize;
static uint8_t _backlightval;
static uint8_t _row_offsets[4];


void LiquidCrystal_I2C_init(uint8_t lcd_addr, uint8_t lcd_cols, uint8_t lcd_rows, uint8_t charsize)
{
	_addr = lcd_addr;
	_cols = lcd_cols;
	_rows = lcd_rows;
	_charsize = charsize;
	_backlightval = LCD_BACKLIGHT;
}


void LiquidCrystal_I2C_begin3(uint8_t lcd_cols, uint8_t lcd_rows, uint8_t charsize)
{
	_cols = lcd_cols;
	_rows = lcd_rows;
	_charsize = charsize;
	LiquidCrystal_I2C_begin();
}


void LiquidCrystal_I2C_begin2(uint8_t lcd_cols, uint8_t lcd_rows)
{
	_cols = lcd_cols;
	_rows = lcd_rows;
	LiquidCrystal_I2C_begin();
}


void LiquidCrystal_I2C_begin() {
#ifdef USE_WIRE
	Wire_begin();
#else
	I2C_begin();
#endif
	_displayfunction = LCD_4BITMODE | LCD_1LINE | LCD_5x8DOTS;

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

  // SEE PAGE 45/46 FOR INITIALIZATION SPECIFICATION!
  // according to datasheet, we need at least 40ms after power rises above 2.7V
  // before sending commands. Arduino can turn on way before 4.5V so we'll wait 50
//  delayMicroseconds(50000);
  delay(50);
  // Now we pull both RS and R/W low to begin commands
	LiquidCrystal_I2C_expanderWrite(_backlightval);	// reset expander and turn backlight on (Bit 2 =1)
#if 1
	delay(1000);

  //put the LCD into 4 bit mode
    // this is according to the hitachi HD44780 datasheet
    // figure 24, pg 46

    // we start in 8bit mode, try to set 4 bit mode
    LiquidCrystal_I2C_write4bits(0x03 << 4);
    delayMicroseconds(4500); // wait min 4.1ms

    // second try
    LiquidCrystal_I2C_write4bits(0x03 << 4);
    delayMicroseconds(4500); // wait min 4.1ms

    // third go!
    LiquidCrystal_I2C_write4bits(0x03 << 4);
    delayMicroseconds(150);

    // finally, set to 4-bit interface
    LiquidCrystal_I2C_write4bits(0x02 << 4);

    // Send function set command sequence
    LiquidCrystal_I2C_command(LCD_FUNCTIONSET | _displayfunction);
    delayMicroseconds(4500);  // wait more than 4.1ms
/* not sure if these are needed, but keeping it for reference
    // second try
    LiquidCrystal_I2C_command(LCD_FUNCTIONSET | _displayfunction);
    delayMicroseconds(150);

    // third go
    LiquidCrystal_I2C_command(LCD_FUNCTIONSET | _displayfunction);

    // finally, set # lines, font size, etc.
    LiquidCrystal_I2C_command(LCD_FUNCTIONSET | _displayfunction);
*/
    // turn the display on with no cursor or blinking default
    _displaycontrol = LCD_DISPLAYON | LCD_CURSOROFF | LCD_BLINKOFF;
    LiquidCrystal_I2C_display();

    // clear it off
    LiquidCrystal_I2C_clear();

  // Initialize to default text direction (for roman languages)
  _displaymode = LCD_ENTRYLEFT | LCD_ENTRYSHIFTDECREMENT;
  // set the entry mode
    LiquidCrystal_I2C_command(LCD_ENTRYMODESET | _displaymode);

	LiquidCrystal_I2C_home();
#endif
}

/********** high level commands, for the user! */
void LiquidCrystal_I2C_clear()
{
  LiquidCrystal_I2C_command(LCD_CLEARDISPLAY);  // clear display, set cursor position to zero
  delayMicroseconds(2000);  // this command takes a long time!
}

void LiquidCrystal_I2C_home()
{
  LiquidCrystal_I2C_command(LCD_RETURNHOME);  // set cursor position to zero
  delayMicroseconds(2000);  // this command takes a long time!
}

void LiquidCrystal_I2C_setCursor(uint8_t col, uint8_t row)
{
  const size_t max_lines = sizeof(_row_offsets) / sizeof(*_row_offsets);
  if ( row >= max_lines ) {
    row = max_lines - 1;    // we count rows starting w/0
  }
  if ( row >= _rows ) {
    row = _rows - 1;    // we count rows starting w/0
  }

  LiquidCrystal_I2C_command(LCD_SETDDRAMADDR | (col + _row_offsets[row]));
}

// Turn the display on/off (quickly)
void LiquidCrystal_I2C_noDisplay() {
  _displaycontrol &= ~LCD_DISPLAYON;
  LiquidCrystal_I2C_command(LCD_DISPLAYCONTROL | _displaycontrol);
}
void LiquidCrystal_I2C_display() {
  _displaycontrol |= LCD_DISPLAYON;
  LiquidCrystal_I2C_command(LCD_DISPLAYCONTROL | _displaycontrol);
}

// Turns the underline cursor on/off
void LiquidCrystal_I2C_noCursor() {
  _displaycontrol &= ~LCD_CURSORON;
  LiquidCrystal_I2C_command(LCD_DISPLAYCONTROL | _displaycontrol);
}
void LiquidCrystal_I2C_cursor() {
  _displaycontrol |= LCD_CURSORON;
  LiquidCrystal_I2C_command(LCD_DISPLAYCONTROL | _displaycontrol);
}

// Turn on and off the blinking cursor
void LiquidCrystal_I2C_noBlink() {
  _displaycontrol &= ~LCD_BLINKON;
  LiquidCrystal_I2C_command(LCD_DISPLAYCONTROL | _displaycontrol);
}
void LiquidCrystal_I2C_blink() {
  _displaycontrol |= LCD_BLINKON;
  LiquidCrystal_I2C_command(LCD_DISPLAYCONTROL | _displaycontrol);
}

// These commands scroll the display without changing the RAM
void LiquidCrystal_I2C_scrollDisplayLeft(void) {
  LiquidCrystal_I2C_command(LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVELEFT);
}
void LiquidCrystal_I2C_scrollDisplayRight(void) {
  LiquidCrystal_I2C_command(LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVERIGHT);
}

// This is for text that flows Left to Right
void LiquidCrystal_I2C_leftToRight(void) {
  _displaymode |= LCD_ENTRYLEFT;
  LiquidCrystal_I2C_command(LCD_ENTRYMODESET | _displaymode);
}

// This is for text that flows Right to Left
void LiquidCrystal_I2C_rightToLeft(void) {
  _displaymode &= ~LCD_ENTRYLEFT;
  LiquidCrystal_I2C_command(LCD_ENTRYMODESET | _displaymode);
}

// This will 'right justify' text from the cursor
void LiquidCrystal_I2C_autoscroll(void) {
  _displaymode |= LCD_ENTRYSHIFTINCREMENT;
  LiquidCrystal_I2C_command(LCD_ENTRYMODESET | _displaymode);
}

// This will 'left justify' text from the cursor
void LiquidCrystal_I2C_noAutoscroll(void) {
  _displaymode &= ~LCD_ENTRYSHIFTINCREMENT;
  LiquidCrystal_I2C_command(LCD_ENTRYMODESET | _displaymode);
}

// Allows us to fill the first 8 CGRAM locations
// with custom characters
void LiquidCrystal_I2C_createChar(uint8_t location, uint8_t charmap[]) {
  location &= 0x7; // we only have 8 locations 0-7
  LiquidCrystal_I2C_command(LCD_SETCGRAMADDR | (location << 3));
  for (int i=0; i<8; i++) {
    LiquidCrystal_I2C_write(charmap[i]);
  }
}

// Turn the (optional) backlight off/on
void LiquidCrystal_I2C_noBacklight(void) {
	_backlightval=LCD_NOBACKLIGHT;
	LiquidCrystal_I2C_expanderWrite(0);
}

void LiquidCrystal_I2C_backlight(void) {
	_backlightval=LCD_BACKLIGHT;
	LiquidCrystal_I2C_expanderWrite(0);
}
bool LiquidCrystal_I2C_getBacklight() {
  return _backlightval == LCD_BACKLIGHT;
}


/*********** mid level commands, for sending data/cmds */

void LiquidCrystal_I2C_command(uint8_t value) {
  LiquidCrystal_I2C_send(value, LOW);
}

size_t LiquidCrystal_I2C_write(uint8_t value) {
  LiquidCrystal_I2C_send(value, HIGH);
  return 1; // assume sucess
}


/************ low level data pushing commands **********/

// write either command or data, with automatic 4/8-bit selection
static void LiquidCrystal_I2C_send(uint8_t value, uint8_t mode) {
//FIXME: overly verbose. Check how it compiles
	uint8_t highnib=value&0xf0;
	uint8_t lownib=(value<<4)&0xf0;
	LiquidCrystal_I2C_write4bits((highnib)|mode);
	LiquidCrystal_I2C_write4bits((lownib)|mode);
}

static void LiquidCrystal_I2C_write4bits(uint8_t value) {
	LiquidCrystal_I2C_expanderWrite(value);
	LiquidCrystal_I2C_pulseEnable(value);
}

static void LiquidCrystal_I2C_expanderWrite(uint8_t _data) {
#ifdef USE_WIRE
	Wire_beginTransmission(_addr);
	Wire_write((int)(_data) | _backlightval);
	Wire_endTransmission();
#else
        I2C_write(_addr, _data | _backlightval);
#endif
}

static void LiquidCrystal_I2C_pulseEnable(uint8_t _data) {
	LiquidCrystal_I2C_expanderWrite(_data | En);	// En high
	delayMicroseconds(1);		// enable pulse must be >450ns

	LiquidCrystal_I2C_expanderWrite(_data & ~En);	// En low
	delayMicroseconds(50);		// commands need > 37us to settle
}

//FIXME: why? And if, than it should be inline or #define
void LiquidCrystal_I2C_load_custom_character(uint8_t char_num, uint8_t *rows){
	LiquidCrystal_I2C_createChar(char_num, rows);
}

void LiquidCrystal_I2C_setBacklight(uint8_t new_val) {
	if (new_val) {
		LiquidCrystal_I2C_backlight();		// turn backlight on
	} else {
		LiquidCrystal_I2C_noBacklight();		// turn backlight off
	}
}

