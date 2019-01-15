#include "wiring-header.h"

/**
 */
unsigned long micros()
{
#ifdef USE_SPL
	unsigned long m;
	uint8_t t;

	BEGIN_CRITICAL
	m = timer4_overflow_count;
	t = TIM4_GetCounter();

	// check if a fresh update event is still pending
	// if (TIM4->SR1 & 0x01)
	if ((TIM4_GetFlagStatus(TIM4_IT_UPDATE)==SET) && (t < (T4PERIOD-1)))
		m++;
	END_CRITICAL

//	return ((m << 8) + t) * (64 / clockCyclesPerMicrosecond());
//	return ((m*250+t) * (64/16) );	// FIXME: use calculated value
	m *= T4PERIOD;
	m += t;
//	m <<= 2;
	m *= ((T4PRESCALER_FACTOR*1000000L)/(F_CPU));
	return m;
#else
	unsigned long m;
	uint8_t t;

	BEGIN_CRITICAL
	m = timer4_overflow_count;
	t = TIM4->CNTR;

	// check if a fresh update event is still pending
	// if (TIM4->SR1 & 0x01)
	if ((TIM4->SR1 & (uint8_t)TIM4_IT_UPDATE) && (t < (T4PERIOD-1)))
		m++;
	END_CRITICAL

//	return ((m << 8) + t) * (64 / clockCyclesPerMicrosecond());
//	return ((m*250+t) * (64/16) );	// FIXME: use calculated value
	m *= T4PERIOD;
	m += t;
//	m <<= 2;
	m *= ((T4PRESCALER_FACTOR*1000000L)/(F_CPU));
	return m;
#endif
}


