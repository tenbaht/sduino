/*
 Print.c - Base class that provides print() and println()
 Copyright (c) 2008 David A. Mellis.  All right reserved.

 The float functions are moved to a separate file Print-float.c. This way
 we can keep the linker from pulling in all the float functions even when no
 floats are used in the sketch.

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
 Modified 03 August 2015 by Chuck Todd
 Modified 30 December 2016 by Michael Mayer
 */

#ifdef _GCC_
#include <stdint.h>
#include <stdio.h>
#define PROGMEM
#endif

//#include <stdlib.h>
//#include <stdio.h>
//#include <string.h>
#include <math.h>
#include "Arduino.h"

#include "Print.h"

#ifdef _GCC_
#undef printChr
#define printChr(C) putchar(C)
#endif


void printNL(void);

// Public Methods //////////////////////////////////////////////////////////////

size_t Print_print_sn(writefunc_p writefunc, const uint8_t *buffer, size_t size)
{
  size_t n = 0;
  while (size--) {
    if (writefunc(*buffer++)) n++;
    else break;
  }
  return n;
}

size_t Print_print_s(writefunc_p writefunc, const char *str)
{
	size_t		n = 0;
	char c;

	if (!str) return 0;

	while ( c=*str++ ) {	// assignment intented
		if (writefunc(c)) n++;
		else break;
	}
	return n;
}


size_t Print_print_u(writefunc_p writefunc, unsigned long n)
{
	return printNumber(writefunc, n,10);
}

size_t Print_print_i(writefunc_p writefunc, long n)
{
	return printInt(writefunc, n,10);
}

// Variants of the above functions with an added newline //////////////////////

size_t Print_println_sn(writefunc_p writefunc, const uint8_t *buffer, size_t size)
{
	size_t r;

	r = Print_print_sn(writefunc, buffer, size);
	return r + Print_println(writefunc);
}

size_t Print_println_s(writefunc_p writefunc, const char *str)
{
	size_t r;

	r = Print_print_s(writefunc, str);
	return r + Print_println(writefunc);
}

size_t Print_println_u(writefunc_p writefunc, unsigned long n)
{
	size_t r;

	r = printNumber(writefunc, n,10);
	return r + Print_println(writefunc);
}

size_t Print_println_i(writefunc_p writefunc, long n)
{
	size_t r;

	r = printInt(writefunc, n,10);
	return r + Print_println(writefunc);
}

size_t Print_println_ub(writefunc_p writefunc, unsigned long n, uint8_t base)
{
	size_t r;

	r = printNumber(writefunc, n,base);
	return r + Print_println(writefunc);
}

size_t Print_println_ib(writefunc_p writefunc, long n, uint8_t base)
{
	size_t r;

	r = printInt(writefunc, n,base);
	return r + Print_println(writefunc);
}



// (not so) Private Methods /////////////////////////////////////////////////////////////

size_t Print_println(writefunc_p writefunc)
{
	size_t n;

	n  = writefunc(13);
	n += writefunc(10);
	return n;
}


size_t Print_print_ub(writefunc_p writefunc, unsigned long n, uint8_t base)
{
  char buf[8 * sizeof(long) + 1]; // Assumes 8-bit chars plus zero byte.
  char *str = &buf[sizeof(buf) - 1];

  *str = '\0';

  // prevent crash if called with base == 1
  if (base < 2) base = 10;

  do {
    char c = n % base;
    n /= base;

    *--str = c < 10 ? c + '0' : c + 'A' - 10;
  } while(n);

  return Print_print_s(writefunc, str);
}

size_t Print_print_ib(writefunc_p writefunc, long n, uint8_t base)
{
  if (base == 0) {
    return writefunc((unsigned char) n);
  } else if (base == 10) {
    if (n < 0) {
      int t = writefunc('-');
      n = -n;
      return printNumber(writefunc, n, 10) + t;
    }
    return printNumber(writefunc, n, 10);
  } else {
    return printNumber(writefunc, n, base);
  }
}

