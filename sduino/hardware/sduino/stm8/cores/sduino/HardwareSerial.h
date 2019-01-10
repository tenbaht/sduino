/*
  HardwareSerial.h - Hardware serial library for sduino
  Copyright (c) 2016 Michael Mayer

  Plain C version of HardwareSerial.cpp of the Arduino project.
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
  
  Modified 23 November 2006 by David A. Mellis
  Modified 28 September 2010 by Mark Sproul
  Modified 14 August 2012 by Alarus
  Modified 15 December 2016 by Michael Mayer
*/

#ifndef _HARDWARE_SERIAL_H_
#define _HARDWARE_SERIAL_H_

#include <stdlib.h>
#include <Print.h>

// Define config for Serial.begin(baud, config);
/* coding of the bit field: 00sw .pp.
 * wordlength w: 0:8 1:9 (5,6,7 not supported)
 * parity     pp:  0:n 2:e 3:o
 * stopbits   s:  0:1 1:2
 * => 8x1=0x0x, 9x1=0x1x, 8x2=0x2x, 9x2=0x3x
 * => n=0xx0, e=0xx4, o=0xx6
*/
#define SERIAL_8N1 0x00
#define SERIAL_9N1 0x10
#define SERIAL_8N2 0x20
#define SERIAL_9N2 0x30
#define SERIAL_8E1 0x04
#define SERIAL_9E1 0x14
#define SERIAL_8E2 0x24
#define SERIAL_9E2 0x34
#define SERIAL_8O1 0x06
#define SERIAL_9O1 0x16
#define SERIAL_8O2 0x26
#define SERIAL_9O2 0x36

#define MASK_PARITY	0x06
#define MASK_STOPBITS	0x20
#define MASK_DATABITS	0x10


// Public Methods //////////////////////////////////////////////////////////////

uint8_t HardwareSerial(void);
void HardwareSerial_begin(unsigned long baud);
void HardwareSerial_begin_config(unsigned long baud, uint8_t config);

int HardwareSerial_available(void);
int HardwareSerial_read(void);
size_t HardwareSerial_write(uint8_t c);
void HardwareSerial_flush(void);

void HardwareSerial_end(void);


// Pseudo-OO interface: Plain C disguised as almost-C++, thanks to X-Macros
//
// We want to "inherit" the output functions from Print, so include that
// header file as well before we start calling any macros.

#include <xmacro.h>
#include <Print.h>


// Since this is (so far) a singleton class, simple #define statement will work

#define Serial_begin		HardwareSerial_begin
#define Serial_begin_config	HardwareSerial_begin_config
#define Serial_available	HardwareSerial_available
#define Serial_read		HardwareSerial_read
#define Serial_write		HardwareSerial_write
#define Serial_flush		HardwareSerial_flush
#define Serial_end		HardwareSerial_end


// "inherit" all print methods from the Print "class"

XPrintMethods   (HardwareSerial,Serial)

#endif
