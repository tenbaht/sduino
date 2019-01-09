/*
 * test spi functions
 */

#define ARDUINO_MAIN
#include "Arduino.h"
#include "stm8s_it.h"

void pinMode_c(uint8_t pin, uint8_t mode);
void pinMode_asm(uint8_t pin, uint8_t mode);


uint8_t checkresult(uint16_t adr, uint8_t *data)
{
	uint8_t i,ok;

	ok = 1;
	for (i=0; i<3; ++i) {
		if (((uint8_t*)adr)[2+i] != data[i]) ok=0;
	}
	return ok;
}


void main(void)
{
	// expected result: xx xx 00 00 00
	pinMode_asm(PA1,INPUT);
	checkresult(GPIOA_BaseAddress, "\x00\x00\x00");

	// expected result: xx xx 20 20 00
	pinMode_asm(PB5,OUTPUT);
	checkresult(GPIOB_BaseAddress, "\x20\x20\x00");

	// expected result: xx xx 00 10 00
	pinMode_asm(PC4,INPUT_PULLUP);
	checkresult(GPIOC_BaseAddress, "\x00\x10\x00");

	// expected result: xx xx 20 10 00
	pinMode_asm(PC5,OUTPUT_OD);
	checkresult(GPIOC_BaseAddress, "\x20\x10\x00");

	// expected result: xx xx 02 02 02
	pinMode_asm(PD1,OUTPUT_FAST);
	checkresult(GPIOD_BaseAddress, "\x02\x02\x02");

	// expected result: xx xx 42 02 42
	pinMode_asm(PD6,OUTPUT_OD_FAST);
	checkresult(GPIOD_BaseAddress, "\x42\x02\x42");
}

/*
 * empty default IRQ functions to make the linker happy if the
 * respective module is not to linked.
 */

void UART1_TX_IRQHandler(void) __interrupt(ITC_IRQ_UART1_TX){}
void UART1_RX_IRQHandler(void) __interrupt(ITC_IRQ_UART1_RX){}

// define as __naked to avoid the "clr a/dix x,a" prolog
#define IMPLEMENT_ISR(vect, interrupt) \
 void vect(void) __interrupt((interrupt)>>8) __naked { \
    __asm__("iret"); \
  }

IMPLEMENT_ISR(EXTI_PORTA_IRQHandler,		INT_PORTA) /* EXTI PORTA */
IMPLEMENT_ISR(EXTI_PORTB_IRQHandler,		INT_PORTB) /* EXTI PORTB */
IMPLEMENT_ISR(EXTI_PORTC_IRQHandler,		INT_PORTC) /* EXTI PORTC */
IMPLEMENT_ISR(EXTI_PORTD_IRQHandler,		INT_PORTD) /* EXTI PORTD */
IMPLEMENT_ISR(EXTI_PORTE_IRQHandler,		INT_PORTE) /* EXTI PORTE */
IMPLEMENT_ISR(TIM1_CAP_COM_IRQHandler,		INT_TIM1_CAPCOM)
//IMPLEMENT_ISR(TIM1_UPD_OVF_TRG_BRK_IRQHandler,	INT_TIM1_OVF)
//IMPLEMENT_ISR(TIM2_CAP_COM_IRQHandler,		INT_TIM2_CAPCOM)
//IMPLEMENT_ISR(TIM2_UPD_OVF_TRG_BRK_IRQHandler,	INT_TIM2_OVF)

void TIM4_UPD_OVF_IRQHandler(void) __interrupt(ITC_IRQ_TIM4_OVF){}
