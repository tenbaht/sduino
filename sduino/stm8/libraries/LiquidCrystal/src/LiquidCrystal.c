#include <stdio.h>
#include <string.h>
//#include <inttypes.h>
#include "Arduino.h"
#include "LiquidCrystal.h"

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

static void LiquidCrystal_send(uint8_t, uint8_t);
static void LiquidCrystal_write4bits(uint8_t);
static void LiquidCrystal_write8bits(uint8_t);
static void LiquidCrystal_pulseEnable();

static uint8_t _rs_pin; // LOW: command.  HIGH: character.
static uint8_t _rw_pin; // LOW: write to LCD.  HIGH: read from LCD.
static uint8_t _enable_pin; // activated by a HIGH pulse.
static uint8_t _data_pins[8];

static uint8_t _displayfunction;
static uint8_t _displaycontrol;
static uint8_t _displaymode;

static uint8_t _initialized;

static uint8_t _numlines;
static uint8_t _row_offsets[4];


/*
LiquidCrystal::LiquidCrystal(uint8_t rs, uint8_t rw, uint8_t enable,
			     uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3,
			     uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7)
{
  init(0, rs, rw, enable, d0, d1, d2, d3, d4, d5, d6, d7);
}

LiquidCrystal::LiquidCrystal(uint8_t rs, uint8_t enable,
			     uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3,
			     uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7)
{
  init(0, rs, 255, enable, d0, d1, d2, d3, d4, d5, d6, d7);
}

LiquidCrystal::LiquidCrystal(uint8_t rs, uint8_t rw, uint8_t enable,
			     uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3)
{
  init(1, rs, rw, enable, d0, d1, d2, d3, 0, 0, 0, 0);
}

LiquidCrystal::LiquidCrystal(uint8_t rs,  uint8_t enable,
			     uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3)
{
  init(1, rs, 255, enable, d0, d1, d2, d3, 0, 0, 0, 0);
}
*/

void LiquidCrystal_init(uint8_t fourbitmode, uint8_t rs, uint8_t rw, uint8_t enable,
			 uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3,
			 uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7)
{
  _rs_pin = rs;
  _rw_pin = rw;
  _enable_pin = enable;

  _data_pins[0] = d0;
  _data_pins[1] = d1;
  _data_pins[2] = d2;
  _data_pins[3] = d3;
  _data_pins[4] = d4;
  _data_pins[5] = d5;
  _data_pins[6] = d6;
  _data_pins[7] = d7;

  if (fourbitmode)
    _displayfunction = LCD_4BITMODE | LCD_1LINE | LCD_5x8DOTS;
  else
    _displayfunction = LCD_8BITMODE | LCD_1LINE | LCD_5x8DOTS;

  LiquidCrystal_begin(16, 1);
}

void LiquidCrystal_begin_charsize(uint8_t cols, uint8_t lines, uint8_t dotsize) {
  if (lines > 1) {
    _displayfunction |= LCD_2LINE;
  }
  _numlines = lines;

  LiquidCrystal_setRowOffsets(0x00, 0x40, 0x00 + cols, 0x40 + cols);

  // for some 1 line displays you can select a 10 pixel high font
  if ((dotsize != LCD_5x8DOTS) && (lines == 1)) {
    _displayfunction |= LCD_5x10DOTS;
  }

  pinMode(_rs_pin, OUTPUT);
  // we can save 1 pin by not using RW. Indicate by passing 255 instead of pin#
  if (_rw_pin != 255) {
    pinMode(_rw_pin, OUTPUT);
  }
  pinMode(_enable_pin, OUTPUT);

  // Do these once, instead of every time a character is drawn for speed reasons.
  for (int i=0; i<((_displayfunction & LCD_8BITMODE) ? 8 : 4); ++i)
  {
    pinMode(_data_pins[i], OUTPUT);
   }

  // SEE PAGE 45/46 FOR INITIALIZATION SPECIFICATION!
  // according to datasheet, we need at least 40ms after power rises above 2.7V
  // before sending commands. Arduino can turn on way before 4.5V so we'll wait 50
  delayMicroseconds(50000);
  // Now we pull both RS and R/W low to begin commands
  digitalWrite(_rs_pin, LOW);
  digitalWrite(_enable_pin, LOW);
  if (_rw_pin != 255) {
    digitalWrite(_rw_pin, LOW);
  }

  //put the LCD into 4 bit or 8 bit mode
  if (! (_displayfunction & LCD_8BITMODE)) {
    // this is according to the hitachi HD44780 datasheet
    // figure 24, pg 46

    // we start in 8bit mode, try to set 4 bit mode
    LiquidCrystal_write4bits(0x03);
    delayMicroseconds(4500); // wait min 4.1ms

    // second try
    LiquidCrystal_write4bits(0x03);
    delayMicroseconds(4500); // wait min 4.1ms

    // third go!
    LiquidCrystal_write4bits(0x03);
    delayMicroseconds(150);

    // finally, set to 4-bit interface
    LiquidCrystal_write4bits(0x02);
  } else {
    // this is according to the hitachi HD44780 datasheet
    // page 45 figure 23

    // Send function set command sequence
    LiquidCrystal_command(LCD_FUNCTIONSET | _displayfunction);
    delayMicroseconds(4500);  // wait more than 4.1ms

    // second try
    LiquidCrystal_command(LCD_FUNCTIONSET | _displayfunction);
    delayMicroseconds(150);

    // third go
    LiquidCrystal_command(LCD_FUNCTIONSET | _displayfunction);
  }

  // finally, set # lines, font size, etc.
  LiquidCrystal_command(LCD_FUNCTIONSET | _displayfunction);

  // turn the display on with no cursor or blinking default
  _displaycontrol = LCD_DISPLAYON | LCD_CURSOROFF | LCD_BLINKOFF;
  LiquidCrystal_display();

  // clear it off
  LiquidCrystal_clear();

  // Initialize to default text direction (for romance languages)
  _displaymode = LCD_ENTRYLEFT | LCD_ENTRYSHIFTDECREMENT;
  // set the entry mode
  LiquidCrystal_command(LCD_ENTRYMODESET | _displaymode);

}

void LiquidCrystal_setRowOffsets(uint8_t row0, uint8_t row1, uint8_t row2, uint8_t row3)
{
  _row_offsets[0] = row0;
  _row_offsets[1] = row1;
  _row_offsets[2] = row2;
  _row_offsets[3] = row3;
}

/********** high level commands, for the user! */
void LiquidCrystal_clear()
{
  LiquidCrystal_command(LCD_CLEARDISPLAY);  // clear display, set cursor position to zero
  delayMicroseconds(2000);  // this command takes a long time!
}

void LiquidCrystal_home()
{
  LiquidCrystal_command(LCD_RETURNHOME);  // set cursor position to zero
  delayMicroseconds(2000);  // this command takes a long time!
}

void LiquidCrystal_setCursor(uint8_t col, uint8_t row)
{
  const size_t max_lines = sizeof(_row_offsets) / sizeof(*_row_offsets);
  if ( row >= max_lines ) {
    row = max_lines - 1;    // we count rows starting w/0
  }
  if ( row >= _numlines ) {
    row = _numlines - 1;    // we count rows starting w/0
  }

  LiquidCrystal_command(LCD_SETDDRAMADDR | (col + _row_offsets[row]));
}

// Turn the display on/off (quickly)
void LiquidCrystal_noDisplay() {
  _displaycontrol &= ~LCD_DISPLAYON;
  LiquidCrystal_command(LCD_DISPLAYCONTROL | _displaycontrol);
}
void LiquidCrystal_display() {
  _displaycontrol |= LCD_DISPLAYON;
  LiquidCrystal_command(LCD_DISPLAYCONTROL | _displaycontrol);
}

// Turns the underline cursor on/off
void LiquidCrystal_noCursor() {
  _displaycontrol &= ~LCD_CURSORON;
  LiquidCrystal_command(LCD_DISPLAYCONTROL | _displaycontrol);
}
void LiquidCrystal_cursor() {
  _displaycontrol |= LCD_CURSORON;
  LiquidCrystal_command(LCD_DISPLAYCONTROL | _displaycontrol);
}

// Turn on and off the blinking cursor
void LiquidCrystal_noBlink() {
  _displaycontrol &= ~LCD_BLINKON;
  LiquidCrystal_command(LCD_DISPLAYCONTROL | _displaycontrol);
}
void LiquidCrystal_blink() {
  _displaycontrol |= LCD_BLINKON;
  LiquidCrystal_command(LCD_DISPLAYCONTROL | _displaycontrol);
}

// These commands scroll the display without changing the RAM
void LiquidCrystal_scrollDisplayLeft(void) {
  LiquidCrystal_command(LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVELEFT);
}
void LiquidCrystal_scrollDisplayRight(void) {
  LiquidCrystal_command(LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVERIGHT);
}

// This is for text that flows Left to Right
void LiquidCrystal_leftToRight(void) {
  _displaymode |= LCD_ENTRYLEFT;
  LiquidCrystal_command(LCD_ENTRYMODESET | _displaymode);
}

// This is for text that flows Right to Left
void LiquidCrystal_rightToLeft(void) {
  _displaymode &= ~LCD_ENTRYLEFT;
  LiquidCrystal_command(LCD_ENTRYMODESET | _displaymode);
}

// This will 'right justify' text from the cursor
void LiquidCrystal_autoscroll(void) {
  _displaymode |= LCD_ENTRYSHIFTINCREMENT;
  LiquidCrystal_command(LCD_ENTRYMODESET | _displaymode);
}

// This will 'left justify' text from the cursor
void LiquidCrystal_noAutoscroll(void) {
  _displaymode &= ~LCD_ENTRYSHIFTINCREMENT;
  LiquidCrystal_command(LCD_ENTRYMODESET | _displaymode);
}

// Allows us to fill the first 8 CGRAM locations
// with custom characters
void LiquidCrystal_createChar(uint8_t location, uint8_t charmap[]) {
  location &= 0x7; // we only have 8 locations 0-7
  LiquidCrystal_command(LCD_SETCGRAMADDR | (location << 3));
  for (int i=0; i<8; i++) {
    LiquidCrystal_write(charmap[i]);
  }
  // switch back to character output mode
  LiquidCrystal_command(LCD_SETDDRAMADDR);
}


/*********** mid level commands, for sending data/cmds */

void LiquidCrystal_command(uint8_t value) {
  LiquidCrystal_send(value, LOW);
}

size_t LiquidCrystal_write(uint8_t value) {
  LiquidCrystal_send(value, HIGH);
  return 1; // assume sucess
}


/************ low level data pushing commands **********/

// write either command or data, with automatic 4/8-bit selection
static void LiquidCrystal_send(uint8_t value, uint8_t mode) {
  digitalWrite(_rs_pin, mode);

  // if there is a RW pin indicated, set it low to Write
  if (_rw_pin != 255) {
    digitalWrite(_rw_pin, LOW);
  }

  if (_displayfunction & LCD_8BITMODE) {
    LiquidCrystal_write8bits(value);
  } else {
    LiquidCrystal_write4bits(value>>4);
    LiquidCrystal_write4bits(value);
  }
}

static void LiquidCrystal_pulseEnable(void) {
  digitalWrite(_enable_pin, LOW);
  delayMicroseconds(1);
  digitalWrite(_enable_pin, HIGH);
  delayMicroseconds(1);    // enable pulse must be >450ns
  digitalWrite(_enable_pin, LOW);
  delayMicroseconds(100);   // commands need > 37us to settle
}

static void LiquidCrystal_write4bits(uint8_t value) {
  for (int i = 0; i < 4; i++) {
    digitalWrite(_data_pins[i], (value >> i) & 0x01);
  }

  LiquidCrystal_pulseEnable();
}

static void LiquidCrystal_write8bits(uint8_t value) {
  for (int i = 0; i < 8; i++) {
    digitalWrite(_data_pins[i], (value >> i) & 0x01);
  }

  LiquidCrystal_pulseEnable();
}
