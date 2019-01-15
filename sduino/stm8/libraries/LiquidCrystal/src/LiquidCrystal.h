#ifndef LiquidCrystal_h
#define LiquidCrystal_h


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

#else

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
//
// We want to "inherit" the output functions from Print, so include that
// header file as well before we start calling any macros.

#include <xmacro.h>
#include <Print.h>


/* usage:
 *
 * Instantiation
 * -------------
 *
 * Typically as a global definition. Has to be at the placed in the source
 * file before any of the "methods" can be used. Supports polymorphism.
 *
 * Variants are
 *	8 bit mode, R/W access
 *	8 bit mode, write only access
 *	4 bit mode, R/W access
 *	4 bit mode, write only access
 *
 * Syntax:
 *	LiquidCrystal (instancename,RS,RW,EN,D0,D1,D2,D3,D4,D5,D6,D7)
 *	LiquidCrystal (instancename,RS,EN,D0,D1,D2,D3,D4,D5,D6,D7)
 *	LiquidCrystal (instancename,RS,RW,EN,D0,D1,D2,D3)
 *	LiquidCrystal (instancename,RS,EN,D0,D1,D2,D3)
 *
 *
 * Constructors (typically in the setup() function)
 * ------------------------------------------------
 *
 * Different constructors to emulate the polymorph class constructor
 *
 *      instancename_begin(cols, lines);
 *      instancename_begin_charsize(cols, lines, charsize);
 *
 *
 * Methods
 * -------
 *
 *      instancename_home();
 *      instancename_write(val);
 *      etc.
 *
 */



// special macros specific to this "class"

// The first set of macros defines the polymorph variants of the
// constructor function. The names differ only by the number of arguments,
// this way they can be easily called by the instantiation function.
//
// The macro XLiquidCrystalinst acts as a instantiation function. It defines
// a function to remember the pin connections for later use with begin().
// Depending on the number of arguments this function calls the right
// polymorph variant of the constructor function.
//
// The next few macros are the different functions to mimic the polymorph
// begin() method.

// The constructor functions sorted by the number of arguments.
// For a singleton class these simple #define statements are sufficient.
#define XLiquidCrystal_inst11(RS,RW,EN,D0,D1,D2,D3,D4,D5,D6,D7) \
	LiquidCrystal_init(0,RS,RW,EN,D0,D1,D2,D3,D4,D5,D6,D7)

#define XLiquidCrystal_inst10(RS,EN,D0,D1,D2,D3,D4,D5,D6,D7) \
	LiquidCrystal_init(0,RS,255,EN,D0,D1,D2,D3,D4,D5,D6,D7)

#define XLiquidCrystal_inst7(RS,RW,EN,D0,D1,D2,D3) \
	LiquidCrystal_init(1,RS,RW,EN,D0,D1,D2,D3,0,0,0,0)

#define XLiquidCrystal_inst6(RS,EN,D0,D1,D2,D3) \
	LiquidCrystal_init(1,RS,255,EN,D0,D1,D2,D3,0,0,0,0)


// The instantiation function remembers the pin mapping and the name of the
// needed constructor function in a function definition.
#define XLiquidCrystalinst(instance,...) \
        inline void instance##_inst(void){\
		VARFUNC(XLiquidCrystal_inst,__VA_ARGS__);\
	}


// The variants of the polymorph begin() method.
#define XLCDbegin2(instance) inline \
        void instance##_begin(uint8_t cols, uint8_t lines){\
		instance##_inst(); \
		LiquidCrystal_begin_charsize(cols,lines,LCD_5x8DOTS);\
	}

#define XLCDbegin3(instance) inline \
        void instance##_begin_charsize(uint8_t cols, uint8_t lines, uint8_t charsize){\
		instance##_inst(); \
		LiquidCrystal_begin_charsize(cols,lines,charsize);\
	}


// There are unneeded char declarations at the beginning the end of the
// list. The declaration at the beginning is to consume a possible "static"
// or "extern" before the macro call. The "extern char" at the end consumes
// the semicolon following at the end of line after the macro call.
//
// As a useful side effect this declaration will throw a compiler error
// message if the user tries to define two instances within one sketch.
//
// Calling the 'XPrintMethods' macro defines all functions needed for the
// the pseudo-inheritence of all output functions from Print "class".

#define LiquidCrystal(instance,...) \
	char	 	LiquidCrystal; \
	XLiquidCrystalinst	(instance,__VA_ARGS__) \
	\
	XLCDbegin2	(instance)\
	XLCDbegin3	(instance)\
	\
	X2Method0	(LiquidCrystal,instance,clear) \
	X2Method0	(LiquidCrystal,instance,home) \
	X2Method0	(LiquidCrystal,instance,noDisplay) \
	X2Method0	(LiquidCrystal,instance,display) \
	X2Method0	(LiquidCrystal,instance,noBlink) \
	X2Method0	(LiquidCrystal,instance,blink) \
	X2Method0	(LiquidCrystal,instance,noCursor) \
	X2Method0	(LiquidCrystal,instance,cursor) \
	X2Method0	(LiquidCrystal,instance,scrollDisplayLeft) \
	X2Method0	(LiquidCrystal,instance,scrollDisplayRight) \
	X2Method0	(LiquidCrystal,instance,leftToRight) \
	X2Method0	(LiquidCrystal,instance,rightToLeft) \
	X2Method0	(LiquidCrystal,instance,autoscroll) \
	X2Method0	(LiquidCrystal,instance,noAutoscroll) \
	X2Method4	(LiquidCrystal,instance,setRowOffsets,uint8_t,uint8_t,uint8_t,uint8_t) \
	X2Method2	(LiquidCrystal,instance,createChar,uint8_t,uint8_t*) \
	X2Method2	(LiquidCrystal,instance,setCursor,uint8_t,uint8_t) \
	X2Method1return (LiquidCrystal,instance,size_t,write,uint8_t) \
	X2Method1	(LiquidCrystal,instance,command,uint8_t) \
	\
	XPrintMethods	(LiquidCrystal,instance) \
	extern char	LiquidCrystal


#endif /* c interface */

#endif /* LiquidCrystal */
