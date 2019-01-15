/*
 * This is an example for using the i2c library with a monochrome OLED
 * display based on SSD1306 drivers.
 *
 * The display has 128x64 pixel and uses only SCL and SDA for communication,
 * there is no reset pin.
 *
 * The framebuffer needs to be kept in RAM as reading the display is not
 * supported by the driver chips. Since the STM8S103F3 has only 1kB RAM
 * total, we will see the stack contents in the lower part of the display
 * as a wild bit pattern. Using drawPixel() on this memory would mess up
 * the stack contents and would result in an immediate crash. So don't
 * use the lower lines on low memory devices!
 *
 * This code is a stripped down version of the Adafruit_SSD1306 library
 * adoped for use with the STM8S.
 *
 * This library supports only I2C displays using the I2C library, all SPI
 * related code is removed.
 *
 * All dependencies on the Adafruit_GFX library are removed. This simple
 * code only supports pixel based operations using drawPixel().
 *
 * 2017 modified for the STM8S by Michael Mayer
 */


/*********************************************************************
This is a library for our Monochrome OLEDs based on SSD1306 drivers

  Pick one up today in the adafruit shop!
  ------> http://www.adafruit.com/category/63_98

These displays use SPI to communicate, 4 or 5 pins are required to
interface

Adafruit invests time and resources providing this open source code,
please support Adafruit and open-source hardware by purchasing
products from Adafruit!

Written by Limor Fried/Ladyada  for Adafruit Industries.
BSD license, check license.txt for more information
All text above, and the splash screen below must be included in any redistribution
*********************************************************************/

#ifndef _Mini_SSD1306_H_
#define _Mini_SSD1306_H_

//#define bool boolean

#if ARDUINO >= 100
 #include "Arduino.h"
 #define WIRE_WRITE Wire.write
#else
 #include "WProgram.h"
  #define WIRE_WRITE Wire.send
#endif

#if defined(__SAM3X8E__)
 typedef volatile RwReg PortReg;
 typedef uint32_t PortMask;
 #define HAVE_PORTREG
#elif defined(ARDUINO_ARCH_SAMD)
// not supported
#elif defined(ESP8266) || defined(ESP32) || defined(ARDUINO_STM32_FEATHER) || defined(__arc__)
  typedef volatile uint32_t PortReg;
  typedef uint32_t PortMask;
#elif defined(__AVR__)
  typedef volatile uint8_t PortReg;
  typedef uint8_t PortMask;
  #define HAVE_PORTREG
#else
  // chances are its 32 bit so assume that
  typedef volatile uint32_t PortReg;
  typedef uint32_t PortMask;
#endif

//#include <SPI.h>
//#include <Adafruit_GFX.h>

#define BLACK 0
#define WHITE 1
#define INVERSE 2

#define SSD1306_I2C_ADDRESS   0x3C  // 011110+SA0+RW - 0x3C or 0x3D
// Address for 128x32 is 0x3C
// Address for 128x64 is 0x3D (default) or 0x3C (if SA0 is grounded)

/*=========================================================================
    SSD1306 Displays
    -----------------------------------------------------------------------
    The driver is used in multiple displays (128x64, 128x32, etc.).
    Select the appropriate display below to create an appropriately
    sized framebuffer, etc.

    SSD1306_128_64  128x64 pixel display

    SSD1306_128_32  128x32 pixel display

    SSD1306_96_16

    -----------------------------------------------------------------------*/
   #define SSD1306_128_64
//   #define SSD1306_128_32
//   #define SSD1306_96_16
/*=========================================================================*/

#if defined SSD1306_128_64 && defined SSD1306_128_32
  #error "Only one SSD1306 display can be specified at once in SSD1306.h"
#endif
#if !defined SSD1306_128_64 && !defined SSD1306_128_32 && !defined SSD1306_96_16
  #error "At least one SSD1306 display must be specified in SSD1306.h"
#endif

#if defined SSD1306_128_64
  #define SSD1306_LCDWIDTH                  128
  #define SSD1306_LCDHEIGHT                 64
#endif
#if defined SSD1306_128_32
  #define SSD1306_LCDWIDTH                  128
  #define SSD1306_LCDHEIGHT                 32
#endif
#if defined SSD1306_96_16
  #define SSD1306_LCDWIDTH                  96
  #define SSD1306_LCDHEIGHT                 16
#endif

#define WIDTH	SSD1306_LCDWIDTH
#define HEIGHT	SSD1306_LCDHEIGHT

#define SSD1306_SETCONTRAST 0x81
#define SSD1306_DISPLAYALLON_RESUME 0xA4
#define SSD1306_DISPLAYALLON 0xA5
#define SSD1306_NORMALDISPLAY 0xA6
#define SSD1306_INVERTDISPLAY 0xA7
#define SSD1306_DISPLAYOFF 0xAE
#define SSD1306_DISPLAYON 0xAF

#define SSD1306_SETDISPLAYOFFSET 0xD3
#define SSD1306_SETCOMPINS 0xDA

#define SSD1306_SETVCOMDETECT 0xDB

#define SSD1306_SETDISPLAYCLOCKDIV 0xD5
#define SSD1306_SETPRECHARGE 0xD9

#define SSD1306_SETMULTIPLEX 0xA8

#define SSD1306_SETLOWCOLUMN 0x00
#define SSD1306_SETHIGHCOLUMN 0x10

#define SSD1306_SETSTARTLINE 0x40

#define SSD1306_MEMORYMODE 0x20
#define SSD1306_COLUMNADDR 0x21
#define SSD1306_PAGEADDR   0x22

#define SSD1306_COMSCANINC 0xC0
#define SSD1306_COMSCANDEC 0xC8

#define SSD1306_SEGREMAP 0xA0

#define SSD1306_CHARGEPUMP 0x8D

#define SSD1306_EXTERNALVCC 0x1
#define SSD1306_SWITCHCAPVCC 0x2

// Scrolling #defines
#define SSD1306_ACTIVATE_SCROLL 0x2F
#define SSD1306_DEACTIVATE_SCROLL 0x2E
#define SSD1306_SET_VERTICAL_SCROLL_AREA 0xA3
#define SSD1306_RIGHT_HORIZONTAL_SCROLL 0x26
#define SSD1306_LEFT_HORIZONTAL_SCROLL 0x27
#define SSD1306_VERTICAL_AND_RIGHT_HORIZONTAL_SCROLL 0x29
#define SSD1306_VERTICAL_AND_LEFT_HORIZONTAL_SCROLL 0x2A


#ifdef __cplusplus

// C++ interface for use with regular gcc compiler

#else

// plain C interface for use with SDCC

  void Mini_SSD1306_init(int8_t reset);

  void Mini_SSD1306_begin(uint8_t switchvcc, uint8_t i2caddr, bool reset);

  void Mini_SSD1306_clearDisplay(void);
  void Mini_SSD1306_invertDisplay(uint8_t i);
  void Mini_SSD1306_display();

  void Mini_SSD1306_startscrollright(uint8_t start, uint8_t stop);
  void Mini_SSD1306_startscrollleft(uint8_t start, uint8_t stop);

  void Mini_SSD1306_startscrolldiagright(uint8_t start, uint8_t stop);
  void Mini_SSD1306_startscrolldiagleft(uint8_t start, uint8_t stop);
  void Mini_SSD1306_stopscroll(void);

  void Mini_SSD1306_dim(boolean dim);

  void Mini_SSD1306_drawPixel(int16_t x, int16_t y, uint8_t color);

//  void Mini_SSD1306_drawFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color);
//  void Mini_SSD1306_drawFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color);

//  inline void Mini_SSD1306_drawFastVLineInternal(int16_t x, int16_t y, int16_t h, uint16_t color) __attribute__((always_inline));
//  inline void Mini_SSD1306_drawFastHLineInternal(int16_t x, int16_t y, int16_t w, uint16_t color) __attribute__((always_inline));



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
 *     LiquidCrystal_8bit_rw(instancename,RS,RW,EN,D0,D1,D2,D3,D4,D5,D6,D7)
 *     LiquidCrystal_8bit_r (instancename,RS,EN,D0,D1,D2,D3,D4,D5,D6,D7)
 *     LiquidCrystal_4bit_rw(instancename,RS,RW,EN,D0,D1,D2,D3)
 *     LiquidCrystal_4bit_r (instancename,RS,EN,D0,D1,D2,D3)
 * Constructors (typically in the setup() function):
 *   Different constructors to emulate the polymorph class constructor
 *     instancename_begin(cols, lines);
 *     instancename_begin_charsize(cols, lines, charsize);
 * Methods:
 *     instancename_home();
 *     instancename_write(val);
 *     int instancename_read();
 */



// special macros specific to this "class"
//
// The first macro XLCDinst defines a function to remember the pin
// connections for later use with begin().
//
// The next few macros are the different functions to mimic the polymorph
// begin() method.
//
// The last, big macro lists all methods specific to this "class" for easier
// inclusion into the different versions of the final macro later.

#define XMini_SSD1306Inst(instance,RST) inline \
        void instance##_inst(void){\
		Mini_SSD1306_init(RST);\
	}

#define XMini_SSD1306Begin(instance) inline \
        void instance##_begin(uint8_t vccstate, uint8_t i2caddr, bool reset){\
		instance##_inst(); \
		Mini_SSD1306_begin(vccstate,i2caddr,reset);\
	}

#define XMini_SSD1306Methods(instance) \
	XMini_SSD1306Begin	(instance)\
	\
	X2Method0	(Mini_SSD1306,instance,clearDisplay) \
	X2Method1	(Mini_SSD1306,instance,invertDisplay,uint8_t) \
	X2Method0	(Mini_SSD1306,instance,display) \
	X2Method2	(Mini_SSD1306,instance,startscrollright,uint8_t,uint8_t) \
	X2Method2	(Mini_SSD1306,instance,startscrollleft,uint8_t,uint8_t) \
	X2Method2	(Mini_SSD1306,instance,startscrolldiagright,uint8_t,uint8_t) \
	X2Method2	(Mini_SSD1306,instance,startscrolldiagleft,uint8_t,uint8_t) \
	X2Method0	(Mini_SSD1306,instance,stopscroll) \
	X2Method1	(Mini_SSD1306,instance,dim,boolean) \
	X2Method3	(Mini_SSD1306,instance,drawPixel,int16_t,int16_t,uint8_t)


// There is an unneeded external declaration at the beginning and the end of
// the list to consume a possible "static" before the declaration and the
// semicolon following at the end of line after the macro call.
//
// Calling the 'XMini_SSD1306Methos' macro defines all functions specific for this
// "class". Avoids duplication in case of a polymorph instantiation method.
//
// Calling the 'XPrintMethods' macro defines all functions needed for the
// the pseudo-inheritence of all output functions from Print "class".

#define Mini_SSD1306(instance,RST) \
	extern int 		Mini_SSD1306; \
	XMini_SSD1306Inst	(instance,RST) \
	XMini_SSD1306Methods	(instance)\
	extern int 		Mini_SSD1306

// not used (yet):
//	XPrintMethods		(Mini_SSD1306,instance)

#endif /* c interface */

#endif /* _Mini_SSD1306_H_ */
