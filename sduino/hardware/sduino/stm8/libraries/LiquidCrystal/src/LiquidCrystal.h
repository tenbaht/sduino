#ifndef LiquidCrystal_h
#define LiquidCrystal_h

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


// plain C interface for use with SDCC

  void LiquidCrystal_init(uint8_t fourbitmode, uint8_t rs, uint8_t rw, uint8_t enable,
	    uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3,
	    uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7);

// use another #define to emulate the polymorph method begin()
#define LiquidCrystal_begin(C,R) LiquidCrystal_begin_charsize(C,R,LCD_5x8DOTS)
  void LiquidCrystal_begin_charsize(uint8_t cols, uint8_t rows, uint8_t charsize);

  void LiquidCrystal_clear();
  void LiquidCrystal_home();

  void LiquidCrystal_noDisplay();
  void LiquidCrystal_display();
  void LiquidCrystal_noBlink();
  void LiquidCrystal_blink();
  void LiquidCrystal_noCursor();
  void LiquidCrystal_cursor();
  void LiquidCrystal_scrollDisplayLeft();
  void LiquidCrystal_scrollDisplayRight();
  void LiquidCrystal_leftToRight();
  void LiquidCrystal_rightToLeft();
  void LiquidCrystal_autoscroll();
  void LiquidCrystal_noAutoscroll();

  void LiquidCrystal_setRowOffsets(uint8_t row0, uint8_t row1, uint8_t row2, uint8_t row3);
  void LiquidCrystal_createChar(uint8_t, uint8_t[]);
  void LiquidCrystal_setCursor(uint8_t, uint8_t); 
  size_t LiquidCrystal_write(uint8_t);
  void LiquidCrystal_command(uint8_t);


// Pseudo-OO interface: Plain C disguised as almost-C++, thanks to X-Macros
#include <xmacro.h>

/* usage:
 * Instantiation:
 *   Typically as a global definition. Has to be at the placed in the source
 *   file before any of the "methods" can be used.
 *     Servo(instancename);
 * Constructors (typically in the setup() function):
 *   Different constructors to emulate the polymorph class constructor
 *     instancename_attach(pin);
 *     instancename_attach_minmax(pin, min, max);
 * Methods:
 *     instancename_detach();
 *     instancename_write(val);
 *     instancename_writeMicroseconds(val);
 *     int instancename_read();
 *     int instancename_readMicroseconds();
 *     bool instancename_attached();
 */

#define XLCDinst(instance,mode,rs,rw,en,d0,d1,d2,d3,d4,d5,d6,d7) inline \
        void instance##_inst(void)\
        {LiquidCrystal_init(mode,rs,rw,en,d0,d1,d2,d3,d4,d5,d6,d7);}

#define XLCDbegin2(instance) inline \
        void instance##_begin(uint8_t cols, uint8_t lines)\
        {instance##_inst(); LiquidCrystal_begin_charsize(cols,lines,LCD_5x8DOTS);}

#define XLCDbegin3(instance) inline \
        void instance##_begin_charsize(uint8_t cols, uint8_t lines, uint8_t charsize)\
        {instance##_inst(); LiquidCrystal_begin_charsize(cols,lines,charsize);}


// The instantiation macro *must* be first in the list to allow for a
// extern declaration if the global "object" is used in different source code
// modules.
//
// And it is duplicated as an external declaration at the end of the list to
// avoid a compiler syntax error with the following ';'

// The instantiation macro *must* be the last in the list as it is the only
// macro that does not result in a syntax error with the following ';'
#define LiquidCrystal_8bit_rw(instance,RS,RW,EN,D0,D1,D2,D3,D4,D5,D6,D7) \
	XLCDinst	(instance,0,RS,RW,EN,D0,D1,D2,D3,D4,D5,D6,D7) \
	XLCDbegin2	(instance)\
	XLCDbegin3	(instance)\
	XPreMethod0	(LiquidCrystal,instance,clear) \
	XPreMethod0	(LiquidCrystal,instance,home) \
	XPreMethod1return (LiquidCrystal,instance,size_t,write,uint8_t,value) \
	XPreMethod1	(LiquidCrystal,instance,command,uint8_t,value) \
	extern int LiquidCrystal

#define LiquidCrystal_8bit_r(RS,EN,D0,D1,D2,D3,D4,D5,D6,D7) \
	XLCDinst	(instance,0,RS,255,EN,D0,D1,D2,D3,D4,D5,D6,D7) \
	XLCDbegin2	(instance)\
	XLCDbegin3	(instance)\
	XPreMethod0	(LiquidCrystal,instance,clear) \
	XPreMethod0	(LiquidCrystal,instance,home) \
	XPreMethod1return (LiquidCrystal,instance,size_t,write,uint8_t,value) \
	XPreMethod1	(LiquidCrystal,instance,command,uint8_t,value) \
	extern int LiquidCrystal

#define LiquidCrystal_4bit_rw(RS,RW,EN,D0,D1,D2,D3) \
	XLCDinst	(instance,1,RS,RW,EN,D0,D1,D2,D3,0,0,0,0) \
	XLCDbegin2	(instance)\
	XLCDbegin3	(instance)\
	XPreMethod0	(LiquidCrystal,instance,clear) \
	XPreMethod0	(LiquidCrystal,instance,home) \
	XPreMethod1return (LiquidCrystal,instance,size_t,write,uint8_t,value) \
	XPreMethod1	(LiquidCrystal,instance,command,uint8_t,value) \
	extern int LiquidCrystal

#if 0
#define Xprinthelper0(class,instance,name) inline \
        size_t instance##_##name(void)\
        {return Print_##name(class##_write);}
#define Xprinthelper1(class,instance,name,atype1) inline \
        size_t instance##_##name(atype1 arg1)\
        {return Print_##name(class##_write,arg1);}
#define Xprinthelper2(class,instance,name,atype1,atype2) inline \
        size_t instance##_##name(atype1 arg1,atype2 arg2)\
        {return Print_##name(class##_write,arg1,arg2);}
#endif

#define LiquidCrystal_4bit_r(instance,RS,EN,D0,D1,D2,D3) \
	XLCDinst	(instance,1,RS,255,EN,D0,D1,D2,D3,0,0,0,0) \
	XLCDbegin2	(instance)\
	XLCDbegin3	(instance)\
	XPreMethod0	(LiquidCrystal,instance,clear) \
	XPreMethod0	(LiquidCrystal,instance,home) \
	XPreMethod0	(LiquidCrystal,instance,noDisplay) \
	XPreMethod0	(LiquidCrystal,instance,display) \
	XPreMethod0	(LiquidCrystal,instance,noBlink) \
	XPreMethod0	(LiquidCrystal,instance,blink) \
	XPreMethod0	(LiquidCrystal,instance,noCursor) \
	XPreMethod0	(LiquidCrystal,instance,cursor) \
	XPreMethod0	(LiquidCrystal,instance,scrollDisplayLeft) \
	XPreMethod0	(LiquidCrystal,instance,scrollDisplayRight) \
	XPreMethod0	(LiquidCrystal,instance,leftToRight) \
	XPreMethod0	(LiquidCrystal,instance,rightToLeft) \
	XPreMethod0	(LiquidCrystal,instance,autoscroll) \
	XPreMethod0	(LiquidCrystal,instance,noAutoscroll) \
	XPreMethod4	(LiquidCrystal,instance,setRowOffsets,uint8_t,uint8_t,uint8_t,uint8_t) \
	XPreMethod2	(LiquidCrystal,instance,createChar,uint8_t,uint8_t*) \
	XPreMethod2	(LiquidCrystal,instance,setCursor,uint8_t,uint8_t) \
	XPreMethod1return (LiquidCrystal,instance,size_t,write,uint8_t,value) \
	XPreMethod1	(LiquidCrystal,instance,command,uint8_t,value) \
        Xprinthelper1	(LiquidCrystal,instance,print_s,const char*) \
        Xprinthelper2	(LiquidCrystal,instance,print_sn,const uint8_t*,size_t) \
        Xprinthelper1	(LiquidCrystal,instance,print_i,long) \
        Xprinthelper1	(LiquidCrystal,instance,print_u,unsigned long) \
        Xprinthelper2	(LiquidCrystal,instance,print_ib,long,uint8_t) \
        Xprinthelper2	(LiquidCrystal,instance,print_ub,unsigned long,uint8_t) \
        Xprinthelper0	(LiquidCrystal,instance,println) \
        Xprinthelper1	(LiquidCrystal,instance,println_s,const char*) \
        Xprinthelper1	(LiquidCrystal,instance,println_i,long) \
        Xprinthelper1	(LiquidCrystal,instance,println_u,unsigned long) \
	extern int LiquidCrystal



/*
 * simulate the inheritance of Print class:
 * define some function names with similar names to the methods
 * "inheritated" from "class" Print
 */

#if 0
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
