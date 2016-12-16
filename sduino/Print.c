/*
 Print.cpp - Base class that provides print() and println()
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
 
 Modified 23 November 2006 by David A. Mellis
 Modified 03 August 2015 by Chuck Todd
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

size_t printBuf(const uint8_t *buffer, size_t size)
{
  size_t n = 0;
  while (size--) {
    if (printChr(*buffer++)) n++;
    else break;
  }
  return n;
}

size_t printStr(const char *str)
{
	size_t		n = 0;
	char c;

	if (!str) return 0;

	while ( c=*str++ ) {	// assignment intented
		if (printChr(c)) n++;
		else break;
	}
	return n;
}


size_t Print_print_u(unsigned long n)
{
	return printNumber(n,10);
}

size_t Print_print_i(long n)
{
	return printInt(n,10);
}

size_t Print_println_s(char *str)
{
	size_t r;

	r = printStr(str);
	return r + println();
}



size_t Print_println_u(unsigned long n)
{
	size_t r;

	r = printNumber(n,10);
	return r + println();
}

size_t Print_println_i(long n)
{
	size_t r;

	r = printInt(n,10);
	return r + println();
}


/*
size_t Print::print(long n, int base)
{
  if (base == 0) {
    return write(n);
  } else if (base == 10) {
    if (n < 0) {
      int t = print('-');
      n = -n;
      return printNumber(n, 10) + t;
    }
    return printNumber(n, 10);
  } else {
    return printNumber(n, base);
  }
}

size_t Print::print(unsigned long n, int base)
{
  if (base == 0) return write(n);
  else return printNumber(n, base);
}

size_t Print::print(double n, int digits)
{
  return printFloat(n, digits);
}

size_t Print::println(const __FlashStringHelper *ifsh)
{
  size_t n = print(ifsh);
  n += println();
  return n;
}

size_t Print::print(const Printable& x)
{
  return x.printTo(*this);
}

size_t Print::println(void)
{
  return write("\r\n");
}

size_t Print::println(const String &s)
{
  size_t n = print(s);
  n += println();
  return n;
}

size_t Print::println(const char c[])
{
  size_t n = print(c);
  n += println();
  return n;
}

size_t Print::println(char c)
{
  size_t n = print(c);
  n += println();
  return n;
}

size_t Print::println(unsigned char b, int base)
{
  size_t n = print(b, base);
  n += println();
  return n;
}

size_t Print::println(int num, int base)
{
  size_t n = print(num, base);
  n += println();
  return n;
}

size_t Print::println(unsigned int num, int base)
{
  size_t n = print(num, base);
  n += println();
  return n;
}

size_t Print::println(long num, int base)
{
  size_t n = print(num, base);
  n += println();
  return n;
}

size_t Print::println(unsigned long num, int base)
{
  size_t n = print(num, base);
  n += println();
  return n;
}

size_t Print::println(double num, int digits)
{
  size_t n = print(num, digits);
  n += println();
  return n;
}

size_t Print::println(const Printable& x)
{
  size_t n = print(x);
  n += println();
  return n;
}
*/
// Private Methods /////////////////////////////////////////////////////////////

size_t println(void)
{
	printChr(13);
	printChr(10);
	return 2;
}


size_t printNumber(unsigned long n, uint8_t base)
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

  return printStr(str);
}

size_t printInt(long n, uint8_t base)
{
  if (base == 0) {
    return printChr((unsigned char) n);
  } else if (base == 10) {
    if (n < 0) {
      int t = printChr('-');
      n = -n;
      return printNumber(n, 10) + t;
    }
    return printNumber(n, 10);
  } else {
    return printNumber(n, base);
  }
}

/*
size_t Print_printFloat(double number, uint8_t digits) 
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
