/*
 * test spi functions
 */

#include <stdint.h>

#define DDR	2
#define CR1	3
#define CR2	4

#define BIT_CLEAR(REG) \
	ld	a,yh\
	and	a,(REG,x)\
	ld	(REG,x),a

#define BIT_SET(REG) \
	ld	a,yl\
	or	a,(REG,x)\
	ld	(REG,x),a


void pinMode_asm(uint8_t pin, uint8_t mode)
{
	(void)	pin;	// empty code to avoid warning
	(void)	mode;
__asm

;
; position of the parameters on the stack:
; (3,sp)	pin
; (4,sp)	mode
;

;
; Zuordnung pin => bit mask
;

;	pinmode-c.c: 9: uint8_t bit = digitalPinToBitMask(pin);
	clrw	x
	ld	a, (3, sp)
	ld	xl, a
	addw	x, #(_digital_pin_to_bit_mask_PGM + 0)
	ld	a, (x)
	ld	yl,a		; yl = bit
	cpl	a
	ld	yh,a		; yh = ~bit

;
; Zuordung pin => Port-ID
;
;	pinmode-c.c: 10: uint8_t port = digitalPinToPort(pin);
;	pinmode-c.c: 13: if (port == NOT_A_PORT) return;
; es könnte auch subw x, #(NUM_DIGITAL_PINS) sein
	clrw	x
	ld	a, (3, sp)
	ld	xl, a
	addw	x, #(_digital_pin_to_port_PGM + 0)
	ld	a, (x)		; port-ID. Z-Flag wird gesetzt
	jreq	010$


;
; Zuordung port-ID => Port-Addresse
;
; x = (GPIO_TypeDef *) portOutputRegister(port)
;
00102$:
	clrw	x
	sll	a			; 8 bit shift is sufficient
	ld	xl, a
	addw	x, #(_port_to_output_PGM + 0)
	ldw	x, (x)			; jetzt ist gpio in x

;
; jump table/switch statement for mode parameter
;
	ld	a, (4, sp)
	jreq	000$
	dec	a
	jreq	010$
	dec	a
	jreq	020$
	dec	a
	jreq	030$
	dec	a		; there is no case 4
	dec	a
	jreq	050$
	dec	a		; there is no case 6
	dec	a
	jreq	070$

; fallthrough into OUTPUT as the default case

;
; case OUTPUT			// 1: output, push-pull, slow
;	gpio->CR1 |=  bit;
;	gpio->CR2 &= ~bit;
;	gpio->DDR |=  bit;
;
010$:
	sim
//	BIT_SET(CR1)
	ld	a,yl
	or	a,(CR1,x)
	jra 031$

;
; case OUTPUT_OD		// 3: output open drain, slow
;	gpio->CR1 &= ~bit;
;	gpio->CR2 &= ~bit;
;	gpio->DDR |=  bit;
;
030$:
	sim
//	BIT_CLEAR(CR1)
	ld	a,yh
	and	a,(CR1,x)
031$:	ld	(CR1,x),a
	BIT_CLEAR(CR2)
	BIT_SET(DDR)
	rim
	ret

;
; case INPUT			// 0: input, floating
;	gpio->CR2 &= ~bit;	// first: deactivate interrupt
;	gpio->CR1 &= ~bit;	// release top side
;	gpio->DDR &= ~bit;	// now set direction
;
000$:
	sim
	BIT_CLEAR(CR2)		; first: deactivate interrupt
	BIT_CLEAR(CR1)		; release top side
	BIT_CLEAR(DDR)		; now set direction
	rim
001$:	ret

;
; case INPUT_PULLUP		// 2: input, internal pull up active
;	gpio->CR2 &= ~bit;	// first: deactivate interrupt
;	gpio->DDR &= ~bit;	// set direction before
;	gpio->CR1 |=  bit;	// activating the pull up
;
020$:
	sim
	BIT_CLEAR(CR2)		; first: deactivate interrupt
	BIT_CLEAR(DDR)		; set direction before
	BIT_SET(CR1)		; activating the pull up
	rim
	ret

;
; case OUTPUT_FAST		// 5: output push-pull, fast
;	gpio->CR1 |=  bit;
;	gpio->DDR |=  bit;	// direction before setting CR2 to
;	gpio->CR2 |=  bit;	// avoid accidental interrupt
;
050$:
	sim
//	BIT_SET(CR1)
	ld	a,yl
	or	a,(CR1,x)
	jra 071$

;
; case OUTPUT_OD_FAST		// 7: output open drain, fast
;	gpio->CR1 &= ~bit;
;	gpio->DDR |=  bit;	// direction before setting CR2 to
;	gpio->CR2 |=  bit;	// avoid accidental interrupt
;
070$:
	sim
//	BIT_CLEAR(CR1)
	ld	a,yh\
	and	a,(CR1,x)\
071$:	ld	(CR1,x),a
	BIT_SET(DDR)
	BIT_SET(CR2)
	rim

__endasm;
}
