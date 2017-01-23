/*
  Print.h - Base class that provides print() and println()
  Copyright (c) 2008 David A. Mellis.  All right reserved.

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
*/

#ifndef Print_h
#define Print_h

//#include <inttypes.h>
#include <stdio.h> // for size_t


//#include "WString.h"
//#include "Printable.h"

#define DEC 10
#define HEX 16
#define OCT 8
#define BIN 2


// for the function pointer to the actual write function
typedef size_t (*writefunc_p)(char c);

// this function is used for the actual output:
//int HardwareSerial_write(uint8_t c);	// FIXME: this prototype should not be here
//#define printChr(C)	HardwareSerial_write(C)

// the actual used functions, mostly for internal use
size_t printBuf(writefunc_p writefunc, const uint8_t *buffer, size_t size);
size_t printStr(writefunc_p writefunc, const char *str);
size_t printNumber(writefunc_p writefunc, unsigned long n, uint8_t base);
size_t printInt(writefunc_p writefunc, long n, uint8_t base);
size_t println(writefunc_p writefunc);

// the more sophisticated Arduino-Style functions:

//#define Print_write(B,N)	printBuf(B,N)

// variants of the standard Serial.print() function: Separate impementations
// for string, char, unsigned, signed int
//#define Print_print_s(S)	printStr(S)
//#define Print_print_c(C)	printChr(C)
// print unsigned integer values (char, short, int, long) as decimal values
size_t Print_print_u(writefunc_p writefunc, unsigned long n);
// print signed integer values (char, short, int, long) as decimal value
size_t Print_print_i(writefunc_p writefunc, long n);
// print unsigned integer values (char, short, int, long) to base B
//#define Print_print_ub(U,B)	printNumber(U,B)
// print signed integer values (char, short, int, long) to base B
//#define Print_print_ib(I,B)	printInt(I,B)

size_t Print_println_s(writefunc_p writefunc, char *str);
size_t Print_println_u(writefunc_p writefunc, unsigned long n);
size_t Print_println_i(writefunc_p writefunc, long n);

size_t Print_printFloat(writefunc_p writefunc, double number, uint8_t digits);

#endif
