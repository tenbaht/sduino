/*
 * PCD8544 - Interface with Philips PCD8544 (or compatible) LCDs.
 *
 * Copyright (c) 2010 Carlos Rodrigues <cefrodrigues@gmail.com>
 * Ported to C for use with SDCC 2017 Michael Mayer <michael-mayer@gmx.de>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */


#ifndef PCD8544_H
#define PCD8544_H


#if ARDUINO < 100
#include <WProgram.h>
#else
#include <Arduino.h>
#endif


// Chip variants supported (ST7576 is experimental)...
#define CHIP_PCD8544 0
#define CHIP_ST7576  1


#ifdef __cplusplus

// C++ interface for use with regular gcc compiler

class PCD8544: public Print {
    public:
        // All the pins can be changed from the default values...
        PCD8544(unsigned char sclk  = 3,   /* clock       (display pin 2) */
                unsigned char sdin  = 4,   /* data-in     (display pin 3) */
                unsigned char dc    = 5,   /* data select (display pin 4) */
                unsigned char reset = 6,   /* reset       (display pin 8) */
                unsigned char sce   = 7);  /* enable      (display pin 5) */

        // Display initialization (dimensions in pixels)...
        void begin(unsigned char width=84, unsigned char height=48, unsigned char model=CHIP_PCD8544);
        void stop();

        // Erase everything on the display...
        void clear();
        void clearLine();  // ...or just the current line

        // Control the display's power state...
        void setPower(bool on);

        // For compatibility with the LiquidCrystal library...
        void display();
        void noDisplay();

        // Activate white-on-black mode (whole display)...
        void setInverse(bool inverse);

        // Set display contrast level (0-127)...
        void setContrast(unsigned char level);

        // Place the cursor at the start of the current line...
        void home();

        // Place the cursor at position (column, line)...
        void setCursor(unsigned char column, unsigned char line);

        // Assign a user-defined glyph (5x8) to an ASCII character (0-31)...
        void createChar(unsigned char chr, const unsigned char *glyph);

        // Write an ASCII character at the current cursor position (7-bit)...
#if ARDUINO < 100
        virtual void write(uint8_t chr);
#else
        virtual size_t write(uint8_t chr);
#endif

        // Draw a bitmap at the current cursor position...
        void drawBitmap(const unsigned char *data, unsigned char columns, unsigned char lines);

        // Draw a chart element at the current cursor position...
        void drawColumn(unsigned char lines, unsigned char value);

    private:
        unsigned char pin_sclk;
        unsigned char pin_sdin;
        unsigned char pin_dc;
        unsigned char pin_reset;
        unsigned char pin_sce;

        // Chip variant in use...
        unsigned char model;

        // The size of the display, in pixels...
        unsigned char width;
        unsigned char height;

        // Current cursor position...
        unsigned char column;
        unsigned char line;

        // User-defined glyphs (below the ASCII space character)...
        const unsigned char *custom[' '];

        // Send a command or data to the display...
        void send(unsigned char type, unsigned char data);
};


#else

// we want to "inherit" the output functions, so include it
#include "Print.h"

// plain C interface for use with SDCC
/*
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
*/

// All the pins can be changed from the default values...
void PCD8544_connection(
    unsigned char sclk,	/* clock       (display pin 2) */
    unsigned char sdin,	/* data-in     (display pin 3) */
    unsigned char dc,	/* data select (display pin 4) */
    unsigned char reset,	/* reset       (display pin 8) */
    unsigned char sce);	/* enable      (display pin 5) */

// Display initialization (dimensions in pixels)...
#define PCD8544_begin()	PCD8544_begin_full(84,48, CHIP_PCD8544)
#define PCD8544_begin_wh(W,H)	PCD8544_begin_full(W,H, CHIP_PCD8544)
void PCD8544_begin_full(
    unsigned char width,
    unsigned char height,
    unsigned char model);
void PCD8544_stop();

// Erase everything on the display...
void PCD8544_clear();
void PCD8544_clearLine();  // ...or just the current line

// Control the display's power state...
void PCD8544_setPower(bool on);

// For compatibility with the LiquidCrystal library...
void PCD8544_display();
void PCD8544_noDisplay();

// Activate white-on-black mode (whole display)...
void PCD8544_setInverse(bool inverse);

// Set display contrast level (0-127)...
void PCD8544_setContrast(unsigned char level);

// Place the cursor at the start of the current line...
void PCD8544_home();

// Place the cursor at position (column, line)...
void PCD8544_setCursor(unsigned char column, unsigned char line);

// Assign a user-defined glyph (5x8) to an ASCII character (0-31)...
void PCD8544_createChar(unsigned char chr, const unsigned char *glyph);

// Write an ASCII character at the current cursor position (7-bit)...
#if ARDUINO < 100
  void PCD8544_write(uint8_t chr);
#else
  size_t PCD8544_write(uint8_t chr);
#endif

// Draw a bitmap at the current cursor position...
void PCD8544_drawBitmap(const unsigned char *data, unsigned char columns, unsigned char lines);

// Draw a chart element at the current cursor position...
void PCD8544_drawColumn(unsigned char lines, unsigned char value);



/*
 * define alternative names for the functions that look similar to the
 * standard instance name when using the C++ interface with
 * PCD855 lcd();
 */
#define lcd_connection		PCD8544_connection
#define lcd_begin()		PCD8544_begin_full(84,48, CHIP_PCD8544)
#define lcd_begin_wh(W,H)	PCD8544_begin_full(W,H, CHIP_PCD8544)
#define lcd_begin_full		PCD8544_begin_full
#define lcd_clear		PCD8544_clear
#define lcd_clearLine		PCD8544_clearLine
#define lcd_setPower		PCD8544_setPower
#define lcd_display		PCD8544_display
#define lcd_noDisplay		PCD8544_noDisplay
#define lcd_setInverse		PCD8544_setInverse
#define lcd_setContrast		PCD8544_setContrast
#define lcd_home		PCD8544_home
#define lcd_setCursor		PCD8544_setCursor
#define lcd_createChar		PCD8544_createChar
#define lcd_write(C)		PCD8544_write(C)
#define lcd_drawBitmap		PCD8544_drawBitmap
#define lcd_drawColumn		PCD8544_drawColumn

/*
 * simulate the inheritance from class Print:
 * define some function names with similar names to the methods
 * "inheritated" from "class" Print
 */

// variants of the standard lcd.print() function: Separate impementations
// for string, char, unsigned, signed int
#define lcd_print_s(S)		printStr(PCD8544_write,S)
#define lcd_print_c(C)		PCD8544_write(C)

// print signed/unsigned integer values (char, short, int, long) as decimal values
#define lcd_print_i(I)		Print_print_i(PCD8544_write,I)
#define lcd_print_u(U)		Print_print_u(PCD8544_write,U)

// print signed/unsigned integer values (char, short, int, long) to base B
#define lcd_print_ib(I,B)	printInt(PCD8544_write,I,B)
#define lcd_print_ub(U,B)	printNumber(PCD8544_write,U,B)


#define lcd_println_s(S)	Print_println_s(PCD8544_write,S)
#define lcd_println_u(U)	Print_println_u(PCD8544_write,U)
#define lcd_println_i(I)	Print_println_i(PCD8544_write,I)

// float (not implemented yet)
#define lcd_print_f(F,D)	Print_printFloat(PCD8544_write,F,D)




#endif /* c interface */

#endif  /* PCD8544_H */


/* vim: set expandtab ts=4 sw=4: */
