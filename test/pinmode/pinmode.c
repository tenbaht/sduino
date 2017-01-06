/*
 * test spi functions
 */

#include "Arduino.h"
#include "wiring_private.h"

void pinMode1(uint8_t pin, uint8_t mode)
{
	uint8_t bit = digitalPinToBitMask(pin);
	uint8_t port = digitalPinToPort(pin);
	volatile GPIO_TypeDef *gpio;

	if (port == NOT_A_PIN) return;

	gpio = (GPIO_TypeDef *) portOutputRegister(port);

	if (mode == INPUT) {
		BEGIN_CRITICAL
		gpio->CR2 &= ~bit;	// first: deactivate interrupt
		gpio->CR1 &= ~bit;	// release top side
		gpio->DDR &= ~bit;	// now set direction
		END_CRITICAL
	} else if (mode == INPUT_PULLUP) {
		BEGIN_CRITICAL
		gpio->CR2 &= ~bit;	// first: deactivate interrupt
		gpio->DDR &= ~bit;	// set direction before
		gpio->CR1 |=  bit;	// activating the pull up
		END_CRITICAL
	} else if (mode == OUTPUT_FAST) {// output push-pull, fast
		BEGIN_CRITICAL
		gpio->CR1 |=  bit;
		gpio->DDR |=  bit;	// direction before setting CR2 to
		gpio->CR2 |=  bit;	// avoid accidental interrupt
		END_CRITICAL
	} else if (mode == OUTPUT_OD_FAST) {	// output open drain, fast
		BEGIN_CRITICAL
		gpio->CR1 &= ~bit;
		gpio->DDR |=  bit;	// direction before setting CR2 to
		gpio->CR2 |=  bit;	// avoid accidental interrupt
		END_CRITICAL
	} else if (mode == OUTPUT_OD) {	// output open drain, slow
		BEGIN_CRITICAL
		gpio->CR1 &= ~bit;
		gpio->CR2 &= ~bit;
		gpio->DDR |=  bit;
		END_CRITICAL
	} else {			// output push-pull, slow
		BEGIN_CRITICAL
		gpio->CR1 |=  bit;
		gpio->CR2 &= ~bit;
		gpio->DDR |=  bit;
		END_CRITICAL
	}
}

#if 0
	if (mode == INPUT) {
		gpio->CR2 &= ~bit;	// first: deactivate interrupt
		gpio->CR1 &= ~bit;	// release top side
		gpio->DDR &= ~bit;	// now set direction
	} else if (mode == INPUT_PULLUP) {
		gpio->CR2 &= ~bit;	// first: deactivate interrupt
		gpio->DDR &= ~bit;	// set direction before
		gpio->CR1 |=  bit;	// activating the pull up

	} else if (mode == OUTPUT_FAST) {// output push-pull, fast
		gpio->CR1 |=  bit;
		gpio->DDR |=  bit;	// direction before setting CR2 to
		gpio->CR2 |=  bit;	// avoid accidental interrupt
	} else if (mode == OUTPUT_OD_FAST) {	// output open drain, fast
		gpio->CR1 &= ~bit;
		gpio->DDR |=  bit;	// direction before setting CR2 to
		gpio->CR2 |=  bit;	// avoid accidental interrupt

	} else if (mode == OUTPUT_OD) {	// output open drain, slow
		gpio->CR1 &= ~bit;
		gpio->CR2 &= ~bit;
		gpio->DDR |=  bit;
	} else {			// output push-pull, slow
		gpio->CR1 |=  bit;
		gpio->CR2 &= ~bit;
		gpio->DDR |=  bit;
	}
#endif

#if 0
;	pinmode.c: 10: uint8_t bit = digitalPinToBitMask(pin);
	clrw	x
	ld	a,(0x03, sp)
	ld	xl,a
	addw	x, #_digital_pin_to_bit_mask_PGM+0
	ld	a, (x)		; bit
	ld	yl,a		; yl = bit
	cpl	a
	ld	yh,a		; yh = ~bit

;	pinmode.c: 11: uint8_t port = digitalPinToPort(pin);
	addw	x, #_digital_pin_to_bit_mask_PGM - _digital_pin_to_bit_mask_PGM
	ld	a, (x)		; port
	jreq	00018$

	sll
	clrw
	ld	xl,a
	addw	x, #_port_to_output_PGM+0	; x = gpio

	ld	a,(0x04,sp)	; a = mode, flags are set
#endif
//
void pinMode_asm(uint8_t pin, uint8_t mode)
{
	(void)	pin;	// empty code to avoid warning
	(void)	mode;
__asm
	sub	sp, #16
;	pinmode.c: 10: uint8_t bit = digitalPinToBitMask(pin);
	ldw	x, #_digital_pin_to_bit_mask_PGM+0
	ld	a, xl
	add	a, (0x13, sp)
	rlwa	x
	adc	a, #0x00
	ld	xh, a
	ld	a, (x)
	ld	(0x04, sp), a	; bit := 4
;	pinmode.c: 11: uint8_t port = digitalPinToPort(pin);
	ldw	x, #_digital_pin_to_port_PGM+0
	ld	a, xl
	add	a, (0x13, sp)
	rlwa	x
	adc	a, #0x00
	ld	xh, a
	ld	a, (x)
	ld	(0x03, sp), a
	ld	a, (0x03, sp)
	ld	(0x07, sp), a	; port := 7
;	pinmode.c: 14: if (port == NOT_A_PIN) return;
	tnz	(0x03, sp)
	jrne	00002$
	jp	00018$
00002$:
;	pinmode.c: 16: gpio = (GPIO_TypeDef *) portOutputRegister(port);
	ldw	x, #_port_to_output_PGM+0
	ldw	(0x0e, sp), x
	ld	a, (0x07, sp)
	ld	xl, a
	ld	a, #0x02
	mul	x, a
	addw	x, (0x0e, sp)
	ldw	x, (x)		; jetzt ist gpio in x

	ld	a, (0x04, sp)	; bit
	ld	yl,a		; yl = bit
	cpl	a
	ld	yh,a		; yh = ~bit
	ld	a, (0x14, sp)	; a=mode, flags are set

; gpio->DDR: (2,x) (war an c,SP)
; gpio->CR1: (3,x) (war an 8,SP)
; gpio->CR2: (4,x) (war an 5,SP)

	sim
;	pinmode.c: 18: if (mode == INPUT) {
	jrne	00016$
;	pinmode.c: 20: gpio->CR2 &= ~bit;	// first: deactivate interrupt
	ld	a,yh
	and	a,(4,x)
	ld	(4,x),a
;	pinmode.c: 21: gpio->CR1 &= ~bit;	// release top side
	ld	a,yh
	and	a,(3,x)
	ld	(3,x),a
;	pinmode.c: 22: gpio->DDR &= ~bit;	// now set direction
	ld	a,yh
	and	a,(2,x)
	jp	00022$

00016$:
;	pinmode.c: 24: } else if (mode == INPUT_PULLUP) {
	cp	a, #0x02
	jrne	00013$
;	pinmode.c: 26: gpio->CR2 &= ~bit;	// first: deactivate interrupt
	ld	a,yh
	and	a,(4,x)
	ld	(4,x),a
;	pinmode.c: 27: gpio->DDR &= ~bit;	// set direction before
	ld	a,yh
and	a,(2,x)
	ld	(2,x),a
;	pinmode.c: 28: gpio->CR1 |=  bit;	// activating the pull up
	ld	a,yl
	or	a,(3,x)
	ld	(3,x),a
	jp	00018$

00013$:
;	pinmode.c: 30: } else if (mode == OUTPUT_FAST) {// output push-pull, fast
	cp	a, #0x05
	jrne	00010$
;	pinmode.c: 32: gpio->CR1 |=  bit;
	ld	a,yl
	or	a,(3,x)
	jra	00020$
;	pinmode.c: 39: gpio->DDR |=  bit;	// direction before setting CR2 to
;	pinmode.c: 40: gpio->CR2 |=  bit;	// avoid accidental interrupt
00010$:
;	pinmode.c: 36: } else if (mode == OUTPUT_OD_FAST) {	// output open drain, fast
	cp	a, #0x07
	jrne	00007$
;	pinmode.c: 38: gpio->CR1 &= ~bit;
	ld	a,yh
	and	a,(3,x)
00020$:	ld	(3,x),a
;	pinmode.c: 39: gpio->DDR |=  bit;	// direction before setting CR2 to
	ld	a,yl
	or	a,(2,x)
	ld	(2,x),a
;	pinmode.c: 40: gpio->CR2 |=  bit;	// avoid accidental interrupt
	ld	a,yl
	or	a,(4,x)
	ld	(4,x),a
	jra	00018$

00007$:
;	pinmode.c: 42: } else if (mode == OUTPUT_OD) {	// output open drain, slow
	cp	a, #0x03
	jrne	00004$
;	pinmode.c: 44: gpio->CR1 &= ~bit;
	ld	a,yh
	and	a,(3,x)
	jra	00021$
;	pinmode.c: 45: gpio->CR2 &= ~bit;
;	pinmode.c: 46: gpio->DDR |=  bit;
00004$:
;	pinmode.c: 50: gpio->CR1 |=  bit;
	ld	a,yl
	or	a,(3,x)
00021$:	ld	(3,x),a
;	pinmode.c: 51: gpio->CR2 &= ~bit;
	ld	a,yh
	and	a,(4,x)
	ld	(4,x),a
;	pinmode.c: 52: gpio->DDR |=  bit;
	ld	a,yl
	or	a,(2,x)
00022$:	ld	(2,x),a
00018$:
	rim
	addw	sp, #16
__endasm;
}
//


void setup(void)
{
	// expected result: xx xx 00 00 00
	pinMode_asm(PA1,INPUT);

	// expected result: xx xx 20 20 00
	pinMode_asm(PB5,OUTPUT);

	// expected result: xx xx 00 10 00
	pinMode_asm(PC4,INPUT_PULLUP);

	// expected result: xx xx 20 10 00
	pinMode_asm(PC5,OUTPUT_OD);

	// expected result: xx xx 02 02 02
	pinMode_asm(PD1,OUTPUT_FAST);

	// expected result: xx xx 42 02 42
	pinMode_asm(PD6,OUTPUT_OD_FAST);
}


void loop (void)
{
}
