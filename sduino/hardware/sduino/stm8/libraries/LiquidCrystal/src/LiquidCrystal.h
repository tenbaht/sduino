#ifndef LiquidCrystal_h
#define LiquidCrystal_h

//#include <inttypes.h>	für C++
//#include <stdint.h>	für C
#include "Print.h"

// commands
#define LCD_CLEARDISPLAY 0x01
#define LCD_RETURNHOME 0x02
#define LCD_ENTRYMODESET 0x04
#define LCD_DISPLAYCONTROL 0x08
#define LCD_CURSORSHIFT 0x10
#define LCD_FUNCTIONSET 0x20
#define LCD_SETCGRAMADDR 0x40
#define LCD_SETDDRAMADDR 0x80

// flags for display entry mode
#define LCD_ENTRYRIGHT 0x00
#define LCD_ENTRYLEFT 0x02
#define LCD_ENTRYSHIFTINCREMENT 0x01
#define LCD_ENTRYSHIFTDECREMENT 0x00

// flags for display on/off control
#define LCD_DISPLAYON 0x04
#define LCD_DISPLAYOFF 0x00
#define LCD_CURSORON 0x02
#define LCD_CURSOROFF 0x00
#define LCD_BLINKON 0x01
#define LCD_BLINKOFF 0x00

// flags for display/cursor shift
#define LCD_DISPLAYMOVE 0x08
#define LCD_CURSORMOVE 0x00
#define LCD_MOVERIGHT 0x04
#define LCD_MOVELEFT 0x00

// flags for function set
#define LCD_8BITMODE 0x10
#define LCD_4BITMODE 0x00
#define LCD_2LINE 0x08
#define LCD_1LINE 0x00
#define LCD_5x10DOTS 0x04
#define LCD_5x8DOTS 0x00

#ifdef __cplusplus

// C++ interface for use with regular gcc compiler

class LiquidCrystal : public Print {
public:
  LiquidCrystal(uint8_t rs, uint8_t enable,
		uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3,
		uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7);
  LiquidCrystal(uint8_t rs, uint8_t rw, uint8_t enable,
		uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3,
		uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7);
  LiquidCrystal(uint8_t rs, uint8_t rw, uint8_t enable,
		uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3);
  LiquidCrystal(uint8_t rs, uint8_t enable,
		uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3);

  void init(uint8_t fourbitmode, uint8_t rs, uint8_t rw, uint8_t enable,
	    uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3,
	    uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7);
    
  void begin(uint8_t cols, uint8_t rows, uint8_t charsize = LCD_5x8DOTS);

  void clear();
  void home();

  void noDisplay();
  void display();
  void noBlink();
  void blink();
  void noCursor();
  void cursor();
  void scrollDisplayLeft();
  void scrollDisplayRight();
  void leftToRight();
  void rightToLeft();
  void autoscroll();
  void noAutoscroll();

  void setRowOffsets(int row1, int row2, int row3, int row4);
  void createChar(uint8_t, uint8_t[]);
  void setCursor(uint8_t, uint8_t); 
  virtual size_t write(uint8_t);
  void command(uint8_t);
  
  using Print::write;
private:
  void send(uint8_t, uint8_t);
  void write4bits(uint8_t);
  void write8bits(uint8_t);
  void pulseEnable();

  uint8_t _rs_pin; // LOW: command.  HIGH: character.
  uint8_t _rw_pin; // LOW: write to LCD.  HIGH: read from LCD.
  uint8_t _enable_pin; // activated by a HIGH pulse.
  uint8_t _data_pins[8];

  uint8_t _displayfunction;
  uint8_t _displaycontrol;
  uint8_t _displaymode;

  uint8_t _initialized;

  uint8_t _numlines;
  uint8_t _row_offsets[4];
};

#else

// plain C interface for use with SDCC

// use #defines to emulate the polymorph class constructor in C++
#define lcd_connection_8bit_rw(RS,RW,EN,D0,D1,D2,D3,D4,D5,D6,D7) \
  lcd_init(0,RS,RW,EN,D0,D1,D2,D3,D4,D5,D6,D7)
#define lcd_connection_8bit_r(RS,EN,D0,D1,D2,D3,D4,D5,D6,D7) \
  lcd_init(0,RS,255,EN,D0,D1,D2,D3,D4,D5,D6,D7)
#define lcd_connection_4bit_rw(RS,RW,EN,D0,D1,D2,D3) \
  lcd_init(1,RS,RW,EN,D0,D1,D2,D3,0,0,0,0)
#define lcd_connection_4bit_r(RS,EN,D0,D1,D2,D3) \
  lcd_init(1,RS,255,EN,D0,D1,D2,D3,0,0,0,0)

  void lcd_init(uint8_t fourbitmode, uint8_t rs, uint8_t rw, uint8_t enable,
	    uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3,
	    uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7);

// use another #define to emulate the polymorph method begin()
#define lcd_begin(C,R) lcd_begin_charsize(C,R,LCD_5x8DOTS)
  void lcd_begin_charsize(uint8_t cols, uint8_t rows, uint8_t charsize);

  void lcd_clear();
  void lcd_home();

  void lcd_noDisplay();
  void lcd_display();
  void lcd_noBlink();
  void lcd_blink();
  void lcd_noCursor();
  void lcd_cursor();
  void lcd_scrollDisplayLeft();
  void lcd_scrollDisplayRight();
  void lcd_leftToRight();
  void lcd_rightToLeft();
  void lcd_autoscroll();
  void lcd_noAutoscroll();

  void lcd_setRowOffsets(uint8_t row0, uint8_t row1, uint8_t row2, uint8_t row3);
  void lcd_createChar(uint8_t, uint8_t[]);
  void lcd_setCursor(uint8_t, uint8_t); 
  size_t lcd_write(uint8_t);
  void lcd_command(uint8_t);
  

/*
 * simulate the inheritance of Print class:
 * define some function names with similar names to the methods
 * "inheritated" from "class" Print
 */

// variants of the standard lcd.print() function: Separate impementations
// for string, char, unsigned, signed int
#define lcd_print_s(S)	printStr(lcd_write,S)
#define lcd_print_c(C)	printChr(lcd_write,C)

// print signed/unsigned integer values (char, short, int, long) as decimal values
#define lcd_print_i(I)	Print_print_i(lcd_write,I)
#define lcd_print_u(U)	Print_print_u(lcd_write,U)

// print signed/unsigned integer values (char, short, int, long) to base B
#define lcd_print_ib(I,B)	printInt(lcd_write,I,B)
#define lcd_print_ub(U,B)	printNumber(lcd_write,U,B)


#define lcd_println_s(S)	Print_println_s(lcd_write,S)
#define lcd_println_u(U)	Print_println_u(lcd_write,U)
#define lcd_println_i(I)	Print_println_i(lcd_write,I)

// float (not implemented yet)
#define lcd_print_f(F,D)	Print_printFloat(lcd_write,F,D)


#endif

#endif
