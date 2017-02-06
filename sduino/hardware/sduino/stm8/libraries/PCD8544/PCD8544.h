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

// plain C interface for use with SDCC

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



// Pseudo-OO interface: Plain C disguised as almost-C++, thanks to X-Macros
//
// We want to "inherit" the output functions from Print, so include that
// header file as well before we start calling any macros.

#include <xmacro.h>
#include <Print.h>


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
 *     instancename_write(val);
 *     int instancename_read();
 */



// special macros specific to this "class"
//
// The first macro XPCD8544inst defines a function to remember the pin
// connections for later use with begin().
//
// The next few macros are the different functions to mimic the polymorph
// begin() method.
//
// The last, big macro lists all methods specific to this "class" for easier
// inclusion into the different versions of the final macro later.

#define XPCD8544inst(instance,sclk,sdin,dc,reset,sce) inline \
        void instance##_inst(void){\
            PCD8544_connection(sclk,sdin,dc,reset,sce);\
        }

#define XPCD8544begin(instance) inline \
        void instance##_begin(void){\
            instance##_inst(); \
            PCD8544_begin_full(84,48,CHIP_PCD8544);\
        }

#define XPCD8544begin2(instance) inline \
        void instance##_begin_wh(unsigned char width, unsigned char height){\
            instance##_inst(); \
            PCD8544_begin_full(width,height,CHIP_PCD8544);\
        }

#define XPCD8544begin3(instance) inline \
        void instance##_begin_full(unsigned char width, unsigned char height, unsigned char chip){\
            instance##_inst(); \
            PCD8544_begin_full(width,height,chip);\
        }

#define XPCD8544Methods(instance) \
	XPCD8544begin	(instance) \
	XPCD8544begin2	(instance) \
	XPCD8544begin3	(instance) \
	\
	X2Method0	(PCD8544,instance,stop) \
	X2Method0	(PCD8544,instance,clear) \
	X2Method0	(PCD8544,instance,clearLine) \
	X2Method1	(PCD8544,instance,setPower,bool) \
	X2Method0	(PCD8544,instance,display) \
	X2Method0	(PCD8544,instance,noDisplay) \
	X2Method1	(PCD8544,instance,setInverse,bool) \
	X2Method1	(PCD8544,instance,setContrast,unsigned char) \
	X2Method0	(PCD8544,instance,home) \
	X2Method2	(PCD8544,instance,setCursor,unsigned char,unsigned char) \
	X2Method2	(PCD8544,instance,createChar,unsigned char,const unsigned char*) \
	X2Method1return (PCD8544,instance,size_t,write,uint8_t) \
	X2Method3	(PCD8544,instance,drawBitmap,const unsigned char*,unsigned char,unsigned char) \
	X2Method2	(PCD8544,instance,drawColumn,unsigned char,unsigned char)



// There is an unneeded external declaration at the beginning and the end of
// the list to consume a possible "static" before the declaration and the
// semicolon following at the end of line after the macro call.
//
// Calling the 'XPCD8544Methods' macro defines all functions specific for this
// "class". Avoids duplication in case of a polymorph instantiation method.
//
// Calling the 'XPrintMethods' macro defines all functions needed for the
// the pseudo-inheritence of all output functions from Print "class".

#define PCD8544(instance,sclk,sdin,dc,reset,sce) \
	extern int 	PCD8544; \
	XPCD8544inst	(instance,sclk,sdin,dc,reset,sce) \
	XPCD8544Methods	(instance) \
	XPrintMethods	(PCD8544,instance) \
	extern int 	PCD8544


#endif /* c interface */

#endif  /* PCD8544_H */


/* vim: set expandtab ts=4 sw=4: */
