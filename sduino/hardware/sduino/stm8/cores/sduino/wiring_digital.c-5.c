#include "wiring_digital.c.h"
/**
 * handle the PWM pins
 */
void turnOffPWM(uint8_t timer)
{
    // Output compare mode = 000: Frozen - The comparison between the output
    // compare register TIM1_CCR1 and the counter register TIM1_CNT has no
    // effect on the outputs.
    *((unsigned char *) ccmrx[timer-1]) &= ~TIM1_CCMR_OCM;

    // CCiE = CCiNE = 0: Output disabled (not driven by the timer)
    *((unsigned char *) ccerx[timer-1]) &=~ (DISABLE_TIMER_OUTP_MASK[timer-1]);
}


