#include "wiring_digital.c.h"
/**
 * set the input or output mode of a pin
 */

#ifdef DONT_USE_ASSEMBLER
/***
 * Arduino-style pinMode
 *
 * This version compiles to 270 bytes.
 */
void pinMode(uint8_t pin, uint8_t mode)
{
	uint8_t bit = digitalPinToBitMask(pin);
	uint8_t port = digitalPinToPort(pin);
	volatile GPIO_TypeDef *gpio;

	if (port == NOT_A_PORT) return;

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

#else

/***
 * Hand-crafted and optimized assembler version of pinMode
 *
 * This version compiles to 147 bytes.
 */
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


void pinMode(uint8_t pin, uint8_t mode)
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
; mapping pin => bit mask
;

;	pinmode-c.c: 9: uint8_t bit = digitalPinToBitMask(pin);
	clrw	x
	ld	a, (3, sp)
	ld	xl, a
	pushw	x
	addw	x, #(_digital_pin_to_bit_mask_PGM + 0)
	ld	a, (x)
	ld	yl,a		; yl = bit
	cpl	a
	ld	yh,a		; yh = ~bit

;
; mapping pin => port-ID
;
; The distance between the tables _digital_pin_to_bit_mask_PGM and
; _digital_pin_to_port_PGM is known and constant, but I couldn't find a way
; to do the math through the linker without defining a dummy variable.
;
; The constant NUM_DIGITAL_PINS could be used, but that requires that these
; two tables are always defined right after each other in all pins_arduino.h
; versions. This is possible, but a potential pitfall in the future.
;
; If this distance could be communicated into this module, a simple
; addw x, #(distance) could calculate the table start address.
;
; The second best way is reusing at least the offset part. Only 2 extra
; bytes and full flexibility.

;	pinmode-c.c: 10: uint8_t port = digitalPinToPort(pin);
;	pinmode-c.c: 13: if (port == NOT_A_PORT) return;
	popw	x
	addw	x, #(_digital_pin_to_port_PGM + 0)
	ld	a, (x)		; port-ID. Z-Flag wird gesetzt
	jreq	032$		; if (port == NOT_A_PORT) return;


;
; mapping port-ID => Port-Addresse
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
032$:	ret

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
#endif


/* using an array of pointers compiles way more efficient than doing simple
 * pointer arithmetics like
 *
	if (timer<TIMER21) {
		*(&TIM1->CCMR1 + (timer-TIMER11)) &= 0x8f;
	} else {
		*(&TIM2->CCMR1 + (timer-TIMER21)) &= 0x8f;
	}
 *
 * or a simple switch/case statement like
 *
	switch (timer)
	{
		case TIMER11:   TIM1->CCMR1 &= 0x8f;    break;
		case TIMER12:   TIM1->CCMR2 &= 0x8f;    break;
		case TIMER13:   TIM1->CCMR3 &= 0x8f;    break;
		case TIMER14:   TIM1->CCMR4 &= 0x8f;    break;
		case TIMER21:   TIM2->CCMR1 &= 0x8f;    break;
		case TIMER22:   TIM2->CCMR2 &= 0x8f;    break;
		case TIMER23:   TIM2->CCMR3 &= 0x8f;    break;
	}
 *
 * The most efficient way is this:
 *
#define T1_BASE 0x5258
#define T2_BASE 0x5307
	uint8_t *reg = T1_BASE-1;
	if (timer>4) reg+= (T2_BASE - T1_BASE);
	reg[timer] &= ~TIM1_CCMR_OCM;
 *
 * Unfortunatly, SDCC can't figure out the values TIM1->CCMR1 und TIM->CCMR2
 * early enough in the compile process, so the adresses have to be hardcoded
 * into the code.
 *
 * SDCC is really, really not good in optimizing its code.
 */

