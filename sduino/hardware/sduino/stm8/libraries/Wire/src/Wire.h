/*
  TwoWire.h - TWI/I2C library for Arduino & Wiring
  Copyright (c) 2006 Nicholas Zambetti.  All right reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

  Modified 2012 by Todd Krein (todd@krein.org) to implement repeated starts
  Modified 2017 by Michael Mayer to plain C for use with Sduino
*/

#ifndef TwoWire_h
#define TwoWire_h

#include <stdint.h>
#include <stdbool.h>
//#include "Stream.h"

#define BUFFER_LENGTH 32

// WIRE_HAS_END means Wire has end()
#define WIRE_HAS_END 1

/*
class TwoWire : public Stream
{
  private:
    static uint8_t rxBuffer[];
    static uint8_t rxBufferIndex;
    static uint8_t rxBufferLength;

    static uint8_t txAddress;
    static uint8_t txBuffer[];
    static uint8_t txBufferIndex;
    static uint8_t txBufferLength;

    static uint8_t transmitting;
    static void (*user_onRequest)(void);
    static void (*user_onReceive)(int);
    static void onRequestService(void);
    static void onReceiveService(uint8_t*, int);
  public:
    TwoWire();
    void begin();
    void begin(uint8_t);
    void begin(int);
    void end();
    void setClock(uint32_t);
    void beginTransmission(uint8_t);
    void beginTransmission(int);
    uint8_t endTransmission(void);
    uint8_t endTransmission(uint8_t);
    uint8_t requestFrom(uint8_t, uint8_t);
    uint8_t requestFrom(uint8_t, uint8_t, uint8_t);
    uint8_t requestFrom(uint8_t, uint8_t, uint32_t, uint8_t, uint8_t);
    uint8_t requestFrom(int, int);
    uint8_t requestFrom(int, int, int);
    virtual size_t write(uint8_t);
    virtual size_t write(const uint8_t *, size_t);
    virtual int available(void);
    virtual int read(void);
    virtual int peek(void);
    virtual void flush(void);
    void onReceive( void (*)(int) );
    void onRequest( void (*)(void) );

    inline size_t write(unsigned long n) { return write((uint8_t)n); }
    inline size_t write(long n) { return write((uint8_t)n); }
    inline size_t write(unsigned int n) { return write((uint8_t)n); }
    inline size_t write(int n) { return write((uint8_t)n); }
    using Print::write;
};

extern TwoWire Wire;
*/

/* only this minimal interface is currently implemented: */
void	Wire_begin(void);
void	Wire_end(void);
void	Wire_setClock(uint32_t);
void	Wire_setTimeout(uint16_t);

void		Wire_beginTransmission(uint8_t);
uint8_t 	Wire_endTransmission1(uint8_t sendStop);
inline uint8_t	Wire_endTransmission(void){return Wire_endTransmission1(true);}

size_t	Wire_write(uint8_t);
size_t	Wire_write_s(const uint8_t *);
size_t	Wire_write_sn(const uint8_t *, size_t);
int	Wire_available(void);
int	Wire_read(void);
int	Wire_peek(void);
void	Wire_flush(void);

uint8_t	Wire_requestFrom2(uint8_t address, uint8_t quantity);
uint8_t	Wire_requestFrom3(uint8_t address, uint8_t quantity, uint8_t sendStop);
uint8_t Wire_requestFrom5(uint8_t address, uint8_t quantity, uint32_t iaddress, uint8_t isize, uint8_t sendStop);


// Pseudo-OO interface: Plain C disguised as almost-C++, thanks to X-Macros
//
// We want to "inherit" the output functions from Print, so include that
// header file as well before we start calling any macros.

#include <xmacro.h>
#include <Print.h>

// Since this is a pre-instantiated singleton library we can add some
// polymorphism for the requestFrom() method. Just distinguish by the number
// of arguments.
//#define Wire_write(...) VARFUNC(Wire_write, __VA_ARGS__)
#define Wire_requestFrom(...) VARFUNC(Wire_requestFrom, __VA_ARGS__)

// Unfortunately, this only works for argument numbers > 0. So we can't do the
// same for endTransmission() and endTransmission1(sendStop)
//
// this doesn't work (due to the way sdcc invokes the preprocessor sdcpp):
//#define Wire_endTransmission(...) VARFUNC(Wire_requestFrom, __VA_ARGS__)


// "inherit" all Print methods using the macro defined in xmacro:
//#define TwoWire_write Wire_write1
//XPrintMethods(TwoWire,Wire)
XPrintMethods(Wire,Wire)

#endif
