#ifndef _SERIAL_H_
#define _SERIAL_H_

/*
 * not more than a convininience module to smoothen the transition from
 * real Arduino to sduino syntax for existing sketches.
 *
 * All it does is to include HardwareSerial.h, so it doesn't matter if you
 * include HardwareSerial or Serial into your code.
 *


#include <Arduino.h>
#include <HardwareSerial.h>
#include <Print.h>


// Public Methods //////////////////////////////////////////////////////////////

/*
#define Serial_begin(B)			HardwareSerial_begin(B)
#define Serial_begin_config(B,C)	HardwareSerial_begin_config((B),(C))

#define Serial_available(X)	HardwareSerial_available(X)
#define Serial_read(X)		HardwareSerial_read(X)
#define Serial_write(X)		HardwareSerial_write(X)
#define Serial_flush(X)		HardwareSerial_flush(X)

#define Serial_end(X)		HardwareSerial_end(X)

// the more sophisticated Arduino-Style functions:

// variants of the standard Serial.print() function: Separate impementations
// for string, char, unsigned, signed int
#define Serial_print_s(S)	printStr(HardwareSerial_write,S)
#define Serial_print_c(C)	HardwareSerial_write(C)
#define Serial_print_u(X)	Print_print_u(HardwareSerial_write,X)
#define Serial_print_i(X)	Print_print_i(HardwareSerial_write,X)
#define Serial_print_ub(U,B)	Print_print_ub(HardwareSerial_write,U,B)
#define Serial_print_ib(I,B)	Print_print_ib(HardwareSerial_write,I,B)

#define Serial_println()	Print_println(HardwareSerial_write)
#define Serial_println_s(S)	Print_println_s(HardwareSerial_write,S)
#define Serial_println_u(X)	Print_println_u(X)
#define Serial_println_i(X)	Print_println_i(X)
//#define Serial_println_ub(U,B)	Print_println_ub(U,B)
//#define Serial_println_ib(I,B)	Print_println_ib(I,B)
*/

// Alias Definitions for a more Arduino-like look ////////////////////////////
#if 0
#define Serial_begin		HardwareSerial_begin
#define Serial_begin_config	HardwareSerial_begin_config
#define Serial_available	HardwareSerial_available
#define Serial_read		HardwareSerial_read
#define Serial_write		HardwareSerial_write
#define Serial_flush		HardwareSerial_flush
#define Serial_end		HardwareSerial_end

// variants of the standard Serial.print() function: Separate implementations
// for string, char, unsigned, signed int
#define Serial_print_s(S)	printStr(HardwareSerial_write,S)
#define Serial_print_c(C)	HardwareSerial_write(C)

// print signed/unsigned integer values (char, short, int, long) as decimal values
#define Serial_print_i(I)	Print_print_i(HardwareSerial_write,I)
#define Serial_print_u(U)	Print_print_u(HardwareSerial_write,U)

// print signed/unsigned integer values (char, short, int, long) to base B
#define Serial_print_ib(I,B)	printInt(HardwareSerial_write,I,B)
#define Serial_print_ub(U,B)	printNumber(HardwareSerial_write,U,B)


#define Serial_println()	Print_println(HardwareSerial_write)
#define Serial_println_s(S)	Print_println_s(HardwareSerial_write,S)
#define Serial_println_u(U)	Print_println_u(HardwareSerial_write,U)
#define Serial_println_i(I)	Print_println_i(HardwareSerial_write,I)

// float (not implemented yet)
#define Serial_print_f(F,D)	Print_printFloat(HardwareSerial_write,F,D)
#endif
#endif
