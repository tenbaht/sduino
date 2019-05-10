#include "wiring_digital.c.h"
/**
 * These Bits have to be set to 0 in the timer channel's CCER
 * (Capture/compare enable register) to disable the output, so that the
 * physical pin is not driven by the timer.
 *
 * @see
 * RM0016 Reference Manual
 * STM8S Series and STM8AF Series 8-bit microcontrollers
 * DocID14587 Rev 14 (Oct 2017)
 * Table 38. Output control for complementary OCi and OCiN channels with break
 * feature
 */
const unsigned char DISABLE_TIMER_OUTP_MASK[NUM_TIMERS]={
#ifdef NEED_TIMER_11_12
    (1 << 0) | (1 << 2),    /* for TIMER11 */
    (1 << 4) | (1 << 6),    /* for TIMER12 */
#endif
    (1 << 0) | (1 << 2),    /* for TIMER13 */
    (1 << 4),               /* for TIMER14 */
    (1 << 0),               /* for TIMER21 */
    (1 << 4),               /* for TIMER22 */
#ifdef NEED_TIMER_23
    (1 << 0),               /* for TIMER23 */
#endif
#ifdef NEED_TIMER_31_32
    (1 << 0),               /* for TIMER31 */
    (1 << 4)                /* for TIMER32 */
#endif
};


