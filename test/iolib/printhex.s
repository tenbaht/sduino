;--------------------------------------------------------
; File Created by Michael Mayer
;--------------------------------------------------------
	.module printhex
	.optsdcc -mstm8
	
;--------------------------------------------------------
; Public variables in this module
;--------------------------------------------------------
	.globl	_asm_printhex32
	.globl	_asm_printhex16
	.globl	_asm_printhex8
	.globl	_asm_printhex4

;	.globl	_asm_printdec32
	.globl	_asm_printdec16
	.globl	_asm_printdec8

	.globl	_asm_print16

	.globl	_asm_print_digit
	.globl	_asm_putchar

	.globl	_stacktest
;--------------------------------------------------------
; code
;--------------------------------------------------------
	.area CODE

;
; print a 32 bit value as a 8 digit hexadecimal string
; input: Y=high 16 bits, X=low 16 bits
;
_asm_printhex32:
	pushw	y			; save the high word
	callr	_asm_printhex16		; print the low word
	popw	x			; restore the high word

;
; print a 16 bit value as a 4 digit hexadecimal string
; input: X=16 bit value value
;
_asm_printhex16:
	ld	a,xl
	push	a			; save the low byte
	ld	a,xh
	callr	_asm_printhex8		; print the high byte
	pop	a			; restore the low byte, print it

;
; print a 8 bit value as a 2 digit hexadecimal string
; input: A=8 bit value
;
_asm_printhex8:
	push	a
	swap	a
	callr	_asm_printhex4		; print the high nibble
	pop	a

;
; print a 4 bit value as a single hexadecimal digit
; input: A=4 bit value, upper 4 bits are ignored
;
_asm_printhex4:				; print the low nibble
	and	a,#0x0f

;
; print a single digit. Values >=10 are printed as letters starting with 'A'
; input: A=single digit value, no bits are ignored
;
_asm_print_digit:
	cp	a,#10			; set flag according to a-10
	jrult	00001$			; skip if (a < 10) (jrmi works as well)
	add	a,#('A'-'0')
00001$:
	add	a,#'0'
_asm_putchar:
	ld	0x10,a			; FIXME: only a dummy action
	ret


;_asm_printhex16:
;	ld	a,#16
;	jrt	00002$

;
; print a 8 bit value A to base 10 (note: printdec16 is more efficient)
;
_asm_printdec8:
	clrw	x
	ld	xl,a
	jrt	_asm_printdec16

;
; print a 16 bit value X to base A
;
; Input: X=value to print
; preserved registers: -
;
_asm_print16:
	cp	a,#2
;	jruge	00005$			; ok for base >= 2, otherwise...
	jruge	b05			; ok for base >= 2, otherwise...

;
; print a 16 bit value X to base 10
;
; Input: X=value to print, A=base to use
; preserved registers: -
;
_asm_printdec16:
	ld	a,#10			; ... use base 10
00005$:
b05:
	push	#0xff			; stack end marker
	ld	yl,a			; remember the base
00002$:
	ld	a,yl
	div	x,a			; x=x/a, a=rest
	push	a			; save the digit (does not affect flags)
	jrne	00002$
00004$:	; the number is on the stack in reverse order. pull it out.
	pop	a
	cp	a,#0xff
	jreq	00003$			; end marker found
	callr	_asm_print_digit
	jrt	00004$
00003$:
	ret


_stacktest:
	ret
