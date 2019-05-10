#include "wiring_digital.c.h"
/**
 * timer capture/compare mode register to control PWM mode
 */
const uc_p ccmrx[NUM_TIMERS]={
#ifdef NEED_TIMER_11_12
	TIM1->CCMR1,	/* for TIMER11 */
	TIM1->CCMR2,	/* for TIMER12 */
#endif
	TIM1->CCMR3,	/* for TIMER13 */
	TIM1->CCMR4,	/* for TIMER14 */
	TIM2->CCMR1,	/* for TIMER21 */
	TIM2->CCMR2,	/* for TIMER22 */
#ifdef NEED_TIMER_23
	TIM2->CCMR3,	/* for TIMER23 */
#endif
#ifdef NEED_TIMER_31_32
	TIM3->CCMR1,	/* for TIMER31 */
	TIM3->CCMR2	/* for TIMER32 */
#endif
};

