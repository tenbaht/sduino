/*
 * test the use of the _Generic preprocessor directive
 *
 * It would be very convinient if the compiler could choose between the
 * flexible regular pinMode implementation and an optimized version for
 * constant values.
 */

//#include "Arduino.h"

void pinMode_pin(unsigned char pin, unsigned char mode);

void pinMode_cc(unsigned char pin, unsigned char mode);
void pinMode_cv(unsigned char pin, unsigned char mode);
void pinMode_vc(unsigned char pin, unsigned char mode);
void pinMode_vv(unsigned char pin, unsigned char mode);

#define pinMode_fast(PIN, MODE) if (digitalPinToPort(pin)!=NOT_A_PIN) {\
	(MODE == INPUT) ? (\
	) :(\
	(MODE == OUTPUT) ? () :(\
	)))}

//#define pinMode_cx(PIN,MODE)	pinMode_pin((PIN),(MODE))


#define pinMode_auto(PIN,MODE) _Generic((PIN),\
	const unsigned char:	pinMode_cx((PIN),(MODE)),\
	const char:		pinMode_cx((PIN),(MODE)),\
	const unsigned int:	pinMode_cx((PIN),(MODE)),\
	const int:		pinMode_cx((PIN),(MODE)),\
	default:		pinMode_vx((PIN),(MODE))\
)

#define pinMode_cx(PIN,MODE)	_Generic((MODE),\
	const unsigned char:	pinMode_cc((PIN),(MODE)),\
	const char:		pinMode_cc((PIN),(MODE)),\
	const unsigned int:	pinMode_cc((PIN),(MODE)),\
	const int:		pinMode_cc((PIN),(MODE)),\
	default:		pinMode_cv((PIN),(MODE))\
)

#define pinMode_vx(PIN,MODE)	_Generic((MODE),\
	const unsigned char:	pinMode_vc((PIN),(MODE)),\
	const char:		pinMode_vc((PIN),(MODE)),\
	const unsigned int:	pinMode_vc((PIN),(MODE)),\
	const int:		pinMode_vc((PIN),(MODE)),\
	default:		pinMode_vv((PIN),(MODE))\
)


const char pinnr=2;
const char mode=3;

void test_generic(void)
{
	pinMode_auto(1,1);
	pinMode_auto(1,mode);
//	pinMode_auto(pinnr,1);
//	pinMode_auto(pinnr,mode);

}

#if 0
void pinMode_test(uint8_t pin, uint8_t mode)
{
	uint8_t bit = digitalPinToBitMask(pin);
	uint8_t port = digitalPinToPort(pin);
	volatile GPIO_TypeDef *gpio;

	if (port == NOT_A_PIN) return;

	gpio = (GPIO_TypeDef *) portOutputRegister(port);

	if (mode == INPUT) {
//		BEGIN_CRITICAL
		gpio->CR2 &= ~bit;	// first: deactivate interrupt
		gpio->CR1 &= ~bit;	// release top side
		gpio->DDR &= ~bit;	// now set direction
//		END_CRITICAL
	} else if (mode == INPUT_PULLUP) {
//		BEGIN_CRITICAL
		gpio->CR2 &= ~bit;	// first: deactivate interrupt
		gpio->DDR &= ~bit;	// set direction before
		gpio->CR1 |=  bit;	// activating the pull up
//		END_CRITICAL
	} else if (mode == OUTPUT_FAST) {// output push-pull, fast
//		BEGIN_CRITICAL
		gpio->CR1 |=  bit;
goto output_2;
//		gpio->DDR |=  bit;	// direction before setting CR2 to
//		gpio->CR2 |=  bit;	// avoid accidental interrupt
//		END_CRITICAL
	} else if (mode == OUTPUT_OD_FAST) {	// output open drain, fast
//		BEGIN_CRITICAL
		gpio->CR1 &= ~bit;
output_2:	gpio->DDR |=  bit;	// direction before setting CR2 to
		gpio->CR2 |=  bit;	// avoid accidental interrupt
//		END_CRITICAL
	} else if (mode == OUTPUT_OD) {	// output open drain, slow
//		BEGIN_CRITICAL
		gpio->CR1 &= ~bit;
goto output_3;
//		gpio->CR2 &= ~bit;
//		gpio->DDR |=  bit;
//		END_CRITICAL
	} else {			// output push-pull, slow
//		BEGIN_CRITICAL
		gpio->CR1 |=  bit;
output_3:	gpio->CR2 &= ~bit;
		gpio->DDR |=  bit;
//		END_CRITICAL
	}
}
#endif

#if 0
void pinMode_fast(uint8_t pin, uint8_t mode)
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
#endif
