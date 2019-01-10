#include "wiring_digital.c.h"
/**
 * CCER register for each timer channel.
 *
 * Each 8-bit-register can contain the bits for two channels, so in addition to
 * the register address itself you also need the individual offset(s) of the
 * bit(s) you want to access.
 */
const uc_p ccerx[NUM_TIMERS]={
#ifdef NEED_TIMER_11_12
    TIM1->CCER1,    /* for TIMER11 */
    TIM1->CCER1,    /* for TIMER12 */
#endif
    TIM1->CCER2,    /* for TIMER13 */
    TIM1->CCER2,    /* for TIMER14 */
    TIM2->CCER1,    /* for TIMER21 */
    TIM2->CCER1,    /* for TIMER22 */
#ifdef NEED_TIMER_23
    TIM2->CCER2,    /* for TIMER23 */
#endif
#ifdef NEED_TIMER_31_32
    TIM3->CCER1,    /* for TIMER31 */
    TIM3->CCER1     /* for TIMER32 */
#endif
};

