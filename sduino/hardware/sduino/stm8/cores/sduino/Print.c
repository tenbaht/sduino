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

/*
size_t Print_print_f(writefunc_p writefunc, double number, uint8_t digits) 
{ 
  size_t n = 0;
  uint8_t i;
  unsigned long int_part;
  double remainder, rounding;
  unsigned int toPrint;

  if (isnan(number)) return printStr("nan");
  if (isinf(number)) return printStr("inf");
  if (number > 4294967040.0) return printStr ("ovf");  // constant determined empirically
  if (number <-4294967040.0) return printStr ("ovf");  // constant determined empirically

  // Handle negative numbers
  if (number < 0.0)
  {
     n += printChr('-');
     number = -number;
  }

  // Round correctly so that print(1.999, 2) prints as "2.00"
  rounding = 0.5;
  for (i=0; i<digits; ++i)
    rounding /= 10.0;

  number += rounding;

  // Extract the integer part of the number and print it
  int_part = (unsigned long)number;
  remainder = number - (double)int_part;
  n += printNumber(int_part,10);

  // Print the decimal point, but only if there are digits beyond
  if (digits > 0) {
    n += printChr('.'); 
  }

  // Extract digits from the remainder one at a time
  while (digits-- > 0)
  {
    remainder *= 10.0;
    toPrint = (unsigned int)(remainder);
    n += printNumber(toPrint,10);
    remainder -= toPrint; 
  } 
  
  return n;
}
*/
