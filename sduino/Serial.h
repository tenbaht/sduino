#ifndef _SERIAL_H_
#define _SERIAL_H_

#include <Arduino.h>
#include <HardwareSerial.h>
#include <Print.h>


// Public Methods //////////////////////////////////////////////////////////////

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
#define Serial_print_s(S)	Print_print_s(S)
#define Serial_print_c(C)	Print_print_c(C)
#define Serial_print_u(X)	Print_print_u(X)
#define Serial_print_i(X)	Print_print_i(X)
#define Serial_print_ub(U,B)	Print_print_ub(U,B)
#define Serial_print_ib(I,B)	Print_print_ib(I,B)

#define Serial_println_s(S)	Print_println_s(S)
//#define Serial_println_c(C)	Print_println_c(C)
#define Serial_println_u(X)	Print_println_u(X)
#define Serial_println_i(X)	Print_println_i(X)
//#define Serial_println_ub(U,B)	Print_println_ub(U,B)
//#define Serial_println_ib(I,B)	Print_println_ib(I,B)

#endif
