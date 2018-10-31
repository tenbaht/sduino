#ifndef FDB_LIQUID_CRYSTAL_I2C_H
#define FDB_LIQUID_CRYSTAL_I2C_H

#include "Arduino.h"

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

// flags for backlight control
#define LCD_BACKLIGHT 0x08
#define LCD_NOBACKLIGHT 0x00

#define En B00000100  // Enable bit
#define Rw B00000010  // Read/Write bit
#define Rs B00000001  // Register select bit

/**
 * This is the driver for the Liquid Crystal LCD displays that use the I2C bus.
 *
 * After creating an instance of this class, first call begin() before anything else.
 * The backlight is on by default, since that is the most likely operating mode in
 * most cases.
 */






#ifdef __cplusplus

// C++ interface for use with regular gcc compiler

#else

// plain C interface for use with SDCC

	/**
	 * Constructor
	 *
	 * @param lcd_addr	I2C slave address of the LCD display. Most likely printed on the
	 *					LCD circuit board, or look in the supplied LCD documentation.
	 * @param lcd_cols	Number of columns your LCD display has.
	 * @param lcd_rows	Number of rows your LCD display has.
	 * @param charsize	The size in dots that the display has, use LCD_5x10DOTS or LCD_5x8DOTS.
	 */
	void LiquidCrystal_I2C_init(uint8_t lcd_addr, uint8_t lcd_cols, uint8_t lcd_rows, uint8_t charsize);

	/**
	 * Set the LCD display in the correct begin state, must be called before anything else is done.
	 */
	void LiquidCrystal_I2C_begin();
	void LiquidCrystal_I2C_begin2(uint8_t lcd_cols, uint8_t lcd_rows);
	void LiquidCrystal_I2C_begin3(uint8_t lcd_cols, uint8_t lcd_rows, uint8_t charsize);

	 /**
	  * Remove all the characters currently shown. Next print/write operation will start
	  * from the first position on LCD display.
	  */
	void LiquidCrystal_I2C_clear();

	/**
	 * Next print/write operation will start from the first position on the LCD display.
	 */
	void LiquidCrystal_I2C_home();

	 /**
	  * Do not show any characters on the LCD display. Backlight state will remain unchanged.
	  * Also all characters written on the display will return, when the display in enabled again.
	  */
	void LiquidCrystal_I2C_noDisplay();

	/**
	 * Show the characters on the LCD display, this is the normal behaviour. This method should
	 * only be used after noDisplay() has been used.
	 */
	void LiquidCrystal_I2C_display();

	/**
	 * Do not blink the cursor indicator.
	 */
	void LiquidCrystal_I2C_noBlink();

	/**
	 * Start blinking the cursor indicator.
	 */
	void LiquidCrystal_I2C_blink();

	/**
	 * Do not show a cursor indicator.
	 */
	void LiquidCrystal_I2C_noCursor();

	/**
 	 * Show a cursor indicator, cursor can blink on not blink. Use the
	 * methods blink() and noBlink() for changing cursor blink.
	 */
	void LiquidCrystal_I2C_cursor();

	void LiquidCrystal_I2C_scrollDisplayLeft();
	void LiquidCrystal_I2C_scrollDisplayRight();
	void LiquidCrystal_I2C_printLeft();
	void LiquidCrystal_I2C_printRight();
	void LiquidCrystal_I2C_leftToRight();
	void LiquidCrystal_I2C_rightToLeft();
	void LiquidCrystal_I2C_shiftIncrement();
	void LiquidCrystal_I2C_shiftDecrement();
	void LiquidCrystal_I2C_noBacklight();
	void LiquidCrystal_I2C_backlight();
	bool LiquidCrystal_I2C_getBacklight();
	void LiquidCrystal_I2C_autoscroll();
	void LiquidCrystal_I2C_noAutoscroll();

	void LiquidCrystal_I2C_createChar(uint8_t, uint8_t[]);
	void LiquidCrystal_I2C_setCursor(uint8_t, uint8_t); 
	size_t LiquidCrystal_I2C_write(uint8_t);
	void LiquidCrystal_I2C_command(uint8_t);

	inline void LiquidCrystal_I2C_blink_on() { LiquidCrystal_I2C_blink(); }
	inline void LiquidCrystal_I2C_blink_off() { LiquidCrystal_I2C_noBlink(); }
	inline void LiquidCrystal_I2C_cursor_on() { LiquidCrystal_I2C_cursor(); }
	inline void LiquidCrystal_I2C_cursor_off() { LiquidCrystal_I2C_noCursor(); }

// Compatibility API function aliases
	void LiquidCrystal_I2C_setBacklight(uint8_t new_val);				// alias for backlight() and nobacklight()
	void LiquidCrystal_I2C_load_custom_character(uint8_t char_num, uint8_t *rows);	// alias for createChar()
	void LiquidCrystal_I2C_printstr(const char[]);




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
 * The original version of the LiquidCystal_I2C library requires the
 * definition of the LCD size and the desired character size (8 or 10 pixel
 * height) at the instantiation uses a parameter less begin() method.
 *
 * This differs slightly from the semantic of the regular Arduino
 * LiquidCrystal library using the parallel interface. There, the
 * instantiation defines only the electrical connection (the used pin
 * numbers) and defines the logical properties (cols, rows, charsize) later
 * with the begin method.
 *
 * This port of the library supports both styles.
 *
 * Syntax:
 *	LiquidCrystal_I2C (instancename,i2c_addr,cols,rows,charsize)
 *	LiquidCrystal_I2C (instancename,i2c_addr,cols,rows)
 *	LiquidCrystal_I2C (instancename,i2c_addr)
 *
 *
 * Constructors (typically in the setup() function)
 * ------------------------------------------------
 *
 * Different constructors to emulate the polymorph class constructor
 *
 *      instancename_begin();
 *      instancename_begin_wh(cols, lines);
 *      instancename_begin_full(cols, lines, charsize);
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
#define XLiquidCrystal_I2C_inst1(ADDR) \
	LiquidCrystal_I2C_init(ADDR,16,2,LCD_5x8DOTS)

#define XLiquidCrystal_I2C_inst3(ADDR,COLS,ROWS) \
	LiquidCrystal_I2C_init(ADDR,COLS,ROWS,LCD_5x8DOTS)

#define XLiquidCrystal_I2C_inst4(ADDR,COLS,ROWS,SIZE) \
	LiquidCrystal_I2C_init(ADDR,COLS,ROWS,SIZE)
	

// The instantiation function remembers the I2C parameters and the name of the
// needed constructor function in a function definition.
#define XLiquidCrystal_I2C_inst(instance,...) \
        inline void instance##_inst(void){\
		VARFUNC(XLiquidCrystal_I2C_inst,__VA_ARGS__);\
	}


// The variants of the polymorph begin() method.
// The begin() method includes the delayed call of the inst() method.
#define XLiquidCrystal_I2C_begin0(instance) inline \
        void instance##_begin(void){\
		instance##_inst(); \
		LiquidCrystal_I2C_begin();\
	}

#define XLiquidCrystal_I2C_begin2(instance) inline \
        void instance##_begin_wh(uint8_t cols, uint8_t lines){\
		instance##_inst(); \
		LiquidCrystal_I2C_begin2(cols,lines);\
	}

#define XLiquidCrystal_I2C_begin3(instance) inline \
        void instance##_begin_charsize(uint8_t cols, uint8_t lines, uint8_t charsize){\
		instance##_inst(); \
		LiquidCrystal_I2C_begin3(cols,lines,charsize);\
	}


// There are unneeded char declarations at the beginning and the end of the
// list. The declaration at the beginning is to consume a possible "static"
// or "extern" before the macro call. The "extern char" at the end consumes
// the semicolon following at the end of line after the macro call.
//
// As a useful side effect this declaration will throw a compiler error
// message if the user tries to define two instances within one sketch.
//
// Calling the 'XPrintMethods' macro defines all functions needed for the
// the pseudo-inheritence of all output functions from Print "class".

#define LiquidCrystal_I2C(instance,...) \
	char	 	LiquidCrystal_I2C; \
	XLiquidCrystal_I2C_inst	(instance,__VA_ARGS__) \
	\
	XLiquidCrystal_I2C_begin0	(instance)\
	XLiquidCrystal_I2C_begin2	(instance)\
	XLiquidCrystal_I2C_begin3	(instance)\
	\
	X2Method0	(LiquidCrystal_I2C,instance,clear) \
	X2Method0	(LiquidCrystal_I2C,instance,home) \
	X2Method0	(LiquidCrystal_I2C,instance,noDisplay) \
	X2Method0	(LiquidCrystal_I2C,instance,display) \
	X2Method0	(LiquidCrystal_I2C,instance,noBlink) \
	X2Method0	(LiquidCrystal_I2C,instance,blink) \
	X2Method0	(LiquidCrystal_I2C,instance,noCursor) \
	X2Method0	(LiquidCrystal_I2C,instance,cursor) \
	X2Method0	(LiquidCrystal_I2C,instance,scrollDisplayLeft) \
	X2Method0	(LiquidCrystal_I2C,instance,scrollDisplayRight) \
	X2Method0	(LiquidCrystal_I2C,instance,printLeft) \
	X2Method0	(LiquidCrystal_I2C,instance,printRight) \
	X2Method0	(LiquidCrystal_I2C,instance,leftToRight) \
	X2Method0	(LiquidCrystal_I2C,instance,rightToLeft) \
	X2Method0	(LiquidCrystal_I2C,instance,shiftIncrement) \
	X2Method0	(LiquidCrystal_I2C,instance,shiftDecrement) \
	X2Method0	(LiquidCrystal_I2C,instance,noBacklight) \
	X2Method0	(LiquidCrystal_I2C,instance,backlight) \
	X2Method0return (LiquidCrystal_I2C,instance,bool,getBacklight) \
	X2Method0	(LiquidCrystal_I2C,instance,autoscroll) \
	X2Method0	(LiquidCrystal_I2C,instance,noAutoscroll) \
	X2Method2	(LiquidCrystal_I2C,instance,createChar,uint8_t,uint8_t*) \
	X2Method2	(LiquidCrystal_I2C,instance,setCursor,uint8_t,uint8_t) \
	X2Method1return (LiquidCrystal_I2C,instance,size_t,write,uint8_t) \
	X2Method1	(LiquidCrystal_I2C,instance,command,uint8_t) \
	\
	X2Method0	(LiquidCrystal_I2C,instance,blink_on) \
	X2Method0	(LiquidCrystal_I2C,instance,blink_off) \
	X2Method0	(LiquidCrystal_I2C,instance,cursor_on) \
	X2Method0	(LiquidCrystal_I2C,instance,cursor_off) \
	\
	XPrintMethods	(LiquidCrystal_I2C,instance) \
	extern char	LiquidCrystal_I2C



#endif /* c interface */

#endif // FDB_LIQUID_CRYSTAL_I2C_H
