#include "wiring-header.h"

volatile unsigned long timer4_overflow_count;// = 0;
volatile unsigned long timer4_millis;// = 0;
static unsigned char timer4_fract;// = 0;

//void TIM4_UPD_OVF_IRQHandler(void) __interrupt(ITC_IRQ_TIM4_OVF) /* TIM4 UPD/OVF */
//void TIM4_UPD_OVF_IRQHandler(void) __interrupt(5)//FIXME
INTERRUPT_HANDLER(TIM4_UPD_OVF_IRQHandler, ITC_IRQ_TIM4_OVF) /* TIM4 UPD/OVF */
{
	// copy these to local variables so they can be stored in registers
	// (volatile variables must be read from memory on every access)
	unsigned long m = timer4_millis;
#if (FRACT_INC != 0)
	unsigned char f = timer4_fract;
#endif

	m += MILLIS_INC;
#if (FRACT_INC != 0)
	f += FRACT_INC;
	if (f >= FRACT_MAX) {
		f -= FRACT_MAX;
		m += 1;
	}

	timer4_fract  = f;
#endif
	timer4_millis = m;
	timer4_overflow_count++;

	/* Clear Interrupt Pending bit */
#ifdef USE_SPL
	TIM4_ClearITPendingBit(TIM4_IT_UPDATE);
#else
	TIM4->SR1 = (uint8_t)(~TIM4_IT_UPDATE);
#endif
}

unsigned long millis()
{
	unsigned long m;

	// disable interrupts while we read timer4_millis or we might get an
	// inconsistent value (e.g. in the middle of a write to timer4_millis)
	BEGIN_CRITICAL
	m = timer4_millis;
	END_CRITICAL

	return m;
}
