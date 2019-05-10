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


#include "PCD8544.h"

#if ARDUINO < 100
#include <WProgram.h>
#else
#include <Arduino.h>
#endif


#define PCD8544_CMD  LOW
#define PCD8544_DATA HIGH


/*
 * If this was a ".h", it would get added to sketches when using
 * the "Sketch -> Import Library..." menu on the Arduino IDE...
 */
#include "charset.inc"


// private data and functions

static unsigned char PCD8544_pin_sclk;
static unsigned char PCD8544_pin_sdin;
static unsigned char PCD8544_pin_dc;
static unsigned char PCD8544_pin_reset;
static unsigned char PCD8544_pin_sce;

// Chip variant in use...
static unsigned char PCD8544_model;

// The size of the display, in pixels...
static unsigned char PCD8544_width;
static unsigned char PCD8544_height;

// Current cursor position...
static unsigned char PCD8544_column;
static unsigned char PCD8544_line;

// User-defined glyphs (below the ASCII space character)...
static const unsigned char *PCD8544_custom[' '];

// Send a command or data to the display...
static void PCD8544_send(unsigned char type, unsigned char data);


// --- public functions: --------------------------------------------------

void PCD8544_connection(unsigned char sclk, unsigned char sdin,
                 unsigned char dc, unsigned char reset,
                 unsigned char sce)
{
    PCD8544_pin_sclk	= sclk;
    PCD8544_pin_sdin	= sdin;
    PCD8544_pin_dc	= dc;
    PCD8544_pin_reset	= reset;
    PCD8544_pin_sce	= sce;
}


void PCD8544_begin_full(unsigned char width, unsigned char height, unsigned char model)
{
    PCD8544_width = width;
    PCD8544_height = height;

    // Only two chip variants are currently known/supported...
    PCD8544_model = (model == CHIP_ST7576) ? CHIP_ST7576 : CHIP_PCD8544;

    PCD8544_column = 0;
    PCD8544_line = 0;

    // Sanitize the custom glyphs...
    memset(PCD8544_custom, 0, sizeof(PCD8544_custom));

    // All pins are outputs (these displays cannot be read)...
    pinMode(PCD8544_pin_sclk, OUTPUT);
    pinMode(PCD8544_pin_sdin, OUTPUT);
    pinMode(PCD8544_pin_dc, OUTPUT);
    pinMode(PCD8544_pin_reset, OUTPUT);
    pinMode(PCD8544_pin_sce, OUTPUT);

    // Reset the controller state...
    digitalWrite(PCD8544_pin_reset, HIGH);
    digitalWrite(PCD8544_pin_sce, HIGH);
    digitalWrite(PCD8544_pin_reset, LOW);
    delay(100);
    digitalWrite(PCD8544_pin_reset, HIGH);

    // Set the LCD parameters...
    PCD8544_send(PCD8544_CMD, 0x21);  // extended instruction set control (H=1)
    PCD8544_send(PCD8544_CMD, 0x13);  // bias system (1:48)

    if (PCD8544_model == CHIP_ST7576) {
        PCD8544_send(PCD8544_CMD, 0xe0);  // higher Vop, too faint at default
        PCD8544_send(PCD8544_CMD, 0x05);  // partial display mode
    } else {
        PCD8544_send(PCD8544_CMD, 0xc2);  // default Vop (3.06 + 66 * 0.06 = 7V)
    }

    PCD8544_send(PCD8544_CMD, 0x20);  // extended instruction set control (H=0)
    PCD8544_send(PCD8544_CMD, 0x09);  // all display segments on

    // Clear RAM contents...
    PCD8544_clear();

    // Activate LCD...
    PCD8544_send(PCD8544_CMD, 0x08);  // display blank
    PCD8544_send(PCD8544_CMD, 0x0c);  // normal mode (0x0d = inverse mode)
    delay(100);

    // Place the cursor at the origin...
    PCD8544_send(PCD8544_CMD, 0x80);
    PCD8544_send(PCD8544_CMD, 0x40);
}


void PCD8544_stop()
{
    PCD8544_clear();
    PCD8544_setPower(false);
}


void PCD8544_clear()
{
    PCD8544_setCursor(0, 0);

    for (unsigned short i = 0; i < PCD8544_width * (PCD8544_height/8); i++) {
        PCD8544_send(PCD8544_DATA, 0x00);
    }

    PCD8544_setCursor(0, 0);
}


void PCD8544_clearLine()
{
    PCD8544_setCursor(0, PCD8544_line);

    for (unsigned char i = 0; i < PCD8544_width; i++) {
        PCD8544_send(PCD8544_DATA, 0x00);
    }

    PCD8544_setCursor(0, PCD8544_line);
}


void PCD8544_setPower(bool on)
{
    PCD8544_send(PCD8544_CMD, on ? 0x20 : 0x24);
}


inline void PCD8544_display()
{
    PCD8544_setPower(true);
}


inline void PCD8544_noDisplay()
{
    PCD8544_setPower(false);
}


void PCD8544_setInverse(bool inverse)
{
    PCD8544_send(PCD8544_CMD, inverse ? 0x0d : 0x0c);
}


void PCD8544_setContrast(unsigned char level)
{
    // The PCD8544 datasheet specifies a maximum Vop of 8.5V for safe
    // operation in low temperatures, which limits the contrast level.
    if (PCD8544_model == CHIP_PCD8544 && level > 90) {
        level = 90;  // Vop = 3.06 + 90 * 0.06 = 8.46V
    }

    // The ST7576 datasheet specifies a minimum Vop of 4V.
    if (PCD8544_model == CHIP_ST7576 && level < 36) {
        level = 36;  // Vop = 2.94 + 36 * 0.03 = 4.02V
    }

    PCD8544_send(PCD8544_CMD, 0x21);  // extended instruction set control (H=1)
    PCD8544_send(PCD8544_CMD, 0x80 | (level & 0x7f));
    PCD8544_send(PCD8544_CMD, 0x20);  // extended instruction set control (H=0)
}


void PCD8544_home()
{
    PCD8544_setCursor(0, PCD8544_line);
}


void PCD8544_setCursor(unsigned char column, unsigned char line)
{
    PCD8544_column = (column % PCD8544_width);
    PCD8544_line = (line % (PCD8544_height/9 + 1));

    PCD8544_send(PCD8544_CMD, 0x80 | PCD8544_column);
    PCD8544_send(PCD8544_CMD, 0x40 | PCD8544_line);
}


void PCD8544_createChar(unsigned char chr, const unsigned char *glyph)
{
    // ASCII 0-31 only...
    if (chr >= ' ') {
        return;
    }

    PCD8544_custom[chr] = glyph;
}


#if ARDUINO < 100
void PCD8544_write(uint8_t chr)
#else
size_t PCD8544_write(uint8_t chr)
#endif
{
    const unsigned char *glyph;
    unsigned char pgm_buffer[5];

    // ASCII 7-bit only...
    if (chr >= 0x80) {
#if ARDUINO < 100
        return;
#else
        return 0;
#endif
    }

    if (chr >= ' ') {
        // Regular ASCII characters are kept in flash to save RAM...
        memcpy_P(pgm_buffer, &charset[chr - ' '], sizeof(pgm_buffer));
        glyph = pgm_buffer;
    } else {
        // Custom glyphs, on the other hand, are stored in RAM...
        if (PCD8544_custom[chr]) {
            glyph = PCD8544_custom[chr];
        } else {
            // Default to a space character if unset...
            memcpy_P(pgm_buffer, &charset[0], sizeof(pgm_buffer));
            glyph = pgm_buffer;
        }
    }

    // Output one column at a time...
    for (unsigned char i = 0; i < 5; i++) {
        PCD8544_send(PCD8544_DATA, glyph[i]);
    }

    // One column between characters...
    PCD8544_send(PCD8544_DATA, 0x00);

    // Update the cursor position...
    PCD8544_column = (PCD8544_column + 6) % PCD8544_width;

    if (PCD8544_column == 0) {
        PCD8544_line = (PCD8544_line + 1) % (PCD8544_height/9 + 1);
    }

#if ARDUINO >= 100
    return 1;
#endif
}


void PCD8544_drawBitmap(const unsigned char *data, unsigned char columns, unsigned char lines)
{
    unsigned char scolumn = PCD8544_column;
    unsigned char sline = PCD8544_line;

    // The bitmap will be clipped at the right/bottom edge of the display...
    unsigned char mx = (scolumn + columns > PCD8544_width) ? (PCD8544_width - scolumn) : columns;
    unsigned char my = (sline + lines > PCD8544_height/8) ? (PCD8544_height/8 - sline) : lines;

    for (unsigned char y = 0; y < my; y++) {
        PCD8544_setCursor(scolumn, sline + y);

        for (unsigned char x = 0; x < mx; x++) {
            PCD8544_send(PCD8544_DATA, data[y * columns + x]);
        }
    }

    // Leave the cursor in a consistent position...
    PCD8544_setCursor(scolumn + columns, sline);
}


void PCD8544_drawColumn(unsigned char lines, unsigned char value)
{
    unsigned char scolumn = PCD8544_column;
    unsigned char sline = PCD8544_line;
    unsigned char mark;
    unsigned char b;

    // Keep "value" within range...
    if (value > lines*8) {
        value = lines*8;
    }

    // Find the line where "value" resides...
    mark = (lines*8 - 1 - value)/8;

    // Clear the lines above the mark...
    for (unsigned char line = 0; line < mark; line++) {
        PCD8544_setCursor(scolumn, sline + line);
        PCD8544_send(PCD8544_DATA, 0x00);
    }

    // Compute the byte to draw at the "mark" line...
    b = 0xff;
    for (unsigned char i = 0; i < lines*8 - mark*8 - value; i++) {
        b <<= 1;
    }

    PCD8544_setCursor(scolumn, sline + mark);
    PCD8544_send(PCD8544_DATA, b);

    // Fill the lines below the mark...
    for (unsigned char line = mark + 1; line < lines; line++) {
        PCD8544_setCursor(scolumn, sline + line);
        PCD8544_send(PCD8544_DATA, 0xff);
    }

    // Leave the cursor in a consistent position...
    PCD8544_setCursor(scolumn + 1, sline);
}


static void PCD8544_send(unsigned char type, unsigned char data)
{
    digitalWrite(PCD8544_pin_dc, type);

    digitalWrite(PCD8544_pin_sce, LOW);
    shiftOut(PCD8544_pin_sdin, PCD8544_pin_sclk, MSBFIRST, data);
    digitalWrite(PCD8544_pin_sce, HIGH);
}


/* vim: set expandtab ts=4 sw=4: */
