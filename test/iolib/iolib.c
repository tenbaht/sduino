/*
 */

#include "printhex.h"

#define PRINTHEX16(V)	__asm__("ldw\tx,#" #V "\ncall\t_asm_printhex16")

#define ASMCALL_A(F,A)	__asm__("ld\ta,#" #A "\ncall\t_" #F)
#define ASMCALL_AX(F,A,X)	__asm__("ld\ta,#" #A "\nldw\tx,#" #X "\ncall\t_" #F)
#define ASMCALL_X(F,X)	__asm__("ldw\tx,#" #X "\ncall\t_" #F)
#define ASMCALL_XY(F,X,Y)	__asm__("ldw\tx,#" #X "\nldw\ty,#" #Y "\ncall\t_" #F)

void stacktest(unsigned int v1, unsigned int v2);

void main(void)
{
	stacktest(0x1234, 0x5678);
	__asm__("ldw\tx,#0x1234");
	asm_printhex16(0x1234);
	PRINTHEX16(0x89ab);
	__asm__("ldw\tx,#1234");
	asm_printdec16(1234);
	ASMCALL_X(asm_printdec16, 12345);
}
