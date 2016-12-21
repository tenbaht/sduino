#ifndef _PRINTHEX_H_
#define _PRINTHEX_H_

// functions for base 16 (hexadecimal)
void asm_printhex32(unsigned long val);
void asm_printhex16(unsigned int val);
void asm_printhex8(unsigned char val);
void asm_printhex4(unsigned char val);

// functions for base 10 (decimal)
void asm_printdec16(unsigned int val);	// print a 16 bit value X to base 10
void asm_printdec8(unsigned char val);	// print a 8 bit value A to base 10 (note: printdec16 is more efficient)

// functions for arbitrary base (base 2..26)
void asm_print16(unsigned int val);	// print a 16 bit value X to base A

// functions for single digits
void asm_print_digit(unsigned char val);
void asm_putchar(char c);

#endif
