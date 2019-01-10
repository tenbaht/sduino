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

; my optimized code
; nicht mehr benötigte Stack-Variablen:
; (0x01, sp) war bit
; (0x07, sp) war port-ID
; (0x08, sp) war port-ID

	sub	sp, #8
;	pinmode-c.c: 9: uint8_t bit = digitalPinToBitMask(pin);
	clrw	x
	ld	a, (0x0b, sp)
ld 0x0100,a
	ld	xl, a
	addw	x, #(_digital_pin_to_bit_mask_PGM + 0)
	ld	a, (x)
ld 0x0101,a
	ld	yl,a		; yl = bit
	cpl	a
	ld	yh,a		; yh = ~bit
;	pinmode-c.c: 10: uint8_t port = digitalPinToPort(pin);
; es könnte auch subw x, #(NUM_DIGITAL_PINS) sein
	clrw	x
	ld	a, (0x0b, sp)
	ld	xl, a
	addw	x, #(_digital_pin_to_port_PGM + 0)
	ld	a, (x)		; port-ID. Z-Flag wird gesetzt
;	pinmode-c.c: 13: if (port == NOT_A_PORT) return;
;	jreq	00118$


;
; Zuordung port-ID => Port-Addresse
;

00102$:
;	pinmode-c.c: 15: gpio = (GPIO_TypeDef *) portOutputRegister(port);
;******
	clrw	x
	sll	a			; 8 bit shift is sufficient
	ld	xl, a
	addw	x, #(_port_to_output_PGM + 0)
	ldw	x, (x)			; jetzt ist gpio in x
ldw 0x0102,x


;
; case INPUT
;
;	pinmode-c.c: 17: if (mode == INPUT) {
	ld	a, (0x0c, sp)
	jrne	00116$

;	pinmode-c.c: 19: gpio->CR2 &= ~bit;	// first: deactivate interrupt
;	pinmode-c.c: 20: gpio->CR1 &= ~bit;	// release top side
;	pinmode-c.c: 21: gpio->DDR &= ~bit;	// now set direction
	sim
	BIT_CLEAR(CR2)
	BIT_CLEAR(CR1)
	BIT_CLEAR(DDR)
	rim
	jp	00118$

;
; case INPUT_PULLUP
;
00116$:
;	pinmode-c.c: 23: } else if (mode == INPUT_PULLUP) {
	cp	a, #0x02
	jrne	00113$
;	pinmode-c.c: 25: gpio->CR2 &= ~bit;	// first: deactivate interrupt
;	pinmode-c.c: 26: gpio->DDR &= ~bit;	// set direction before
;	pinmode-c.c: 27: gpio->CR1 |=  bit;	// activating the pull up
	sim
	BIT_CLEAR(CR2)
	BIT_CLEAR(DDR)
	BIT_SET(CR1)
	rim
	jp	00118$

;
; case OUTPUT_FAST
;
00113$:
;	pinmode-c.c: 29: } else if (mode == OUTPUT_FAST) {// output push-pull, fast
	cp	a, #0x05
	jrne	00110$

;	pinmode-c.c: 31: gpio->CR1 |=  bit;
;	pinmode-c.c: 32: gpio->DDR |=  bit;	// direction before setting CR2 to
;	pinmode-c.c: 33: gpio->CR2 |=  bit;	// avoid accidental interrupt
	sim
	BIT_SET(CR1)
	jra 010$

;
; case OUTPUT_OD_FAST
;
00110$:
;	pinmode-c.c: 35: } else if (mode == OUTPUT_OD_FAST) {	// output open drain, fast
	cp	a, #0x07
	jrne	00107$

;	pinmode-c.c: 37: gpio->CR1 &= ~bit;
;	pinmode-c.c: 38: gpio->DDR |=  bit;	// direction before setting CR2 to
;	pinmode-c.c: 39: gpio->CR2 |=  bit;	// avoid accidental interrupt
	sim
	BIT_CLEAR(CR1)
010$:	BIT_SET(DDR)
	BIT_SET(CR2)
	rim
	jra	00118$

;
; case OUTPUT_OD
;
00107$:
;	pinmode-c.c: 41: } else if (mode == OUTPUT_OD) {	// output open drain, slow
	cp	a, #0x03
	jrne	00104$

;	pinmode-c.c: 43: gpio->CR1 &= ~bit;
;	pinmode-c.c: 44: gpio->CR2 &= ~bit;
;	pinmode-c.c: 45: gpio->DDR |=  bit;
	sim
	BIT_CLEAR(CR1)
	jra 004$

;
; case default
;
00104$:
;	pinmode-c.c: 49: gpio->CR1 |=  bit;
;	pinmode-c.c: 50: gpio->CR2 &= ~bit;
;	pinmode-c.c: 51: gpio->DDR |=  bit;
	sim
	BIT_SET(CR1)
004$:	BIT_CLEAR(CR2)
	BIT_SET(DDR)
	rim

00118$:
;	pinmode-c.c: 54: }
	addw	sp, #8
__endasm;
}

/*
void pinMode_asm(uint8_t pin, uint8_t mode)
{
	(void)	pin;	// empty code to avoid warning
	(void)	mode;
__asm
	sub	sp, #16
#if 1
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
;;hier	addw	x, #_digital_pin_to_bit_mask_PGM - _digital_pin_to_bit_mask_PGM
	ld	a, (x)		; port
	jreq	00018$

	sll	a
	clrw	x
	ld	xl,a
	addw	x, #_port_to_output_PGM+0	; x = gpio

	ld	a,(0x04,sp)	; a = mode, flags are set
#else
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
#endif

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
*/
