#include "wiring-header.h"

/**
 */
void init()
{
#ifndef ENABLE_SWIM
	// free the SWIM pin to be used as a general I/O-Pin
	CFG->GCR = CFG_GCR_SWD;
#endif

	// set the clock to 16 MHz
#ifdef USE_SPL
	CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV1);
#else
	CLK->CKDIVR = 0;
#endif
	
	GPIO_DeInit(GPIOA);
	GPIO_DeInit(GPIOB);
	GPIO_DeInit(GPIOC);
	GPIO_DeInit(GPIOD);
	GPIO_DeInit(GPIOE);	// not present on STM8S103
	GPIO_DeInit(GPIOF);	// not present on STM8S103
#if defined(GPIOG)
	GPIO_DeInit(GPIOG);
#endif
#if defined(GPIOH)
	GPIO_DeInit(GPIOH);
#endif
#if defined(GPIOI)
	GPIO_DeInit(GPIOI);
#endif

#if defined(UART1)
	UART1_DeInit();
#else
	UART2_DeInit();
#endif

	// set timer 4 prescale factor and period (typ. @16MHz: 64*250=1ms)
	TIM4_DeInit();
#ifdef USE_SPL
	// set timer 4 prescale factor and period (typ. @16MHz: 64*250=1ms)
	TIM4_TimeBaseInit(T4PRESCALER, (uint8_t) T4PERIOD-1);
	/* Clear TIM4 update flag */
	TIM4_ClearFlag(TIM4_FLAG_UPDATE);	// TIM4->SR1 = (uint8_t)(~0x01);
	/* Enable update interrupt */
	TIM4_ITConfig(TIM4_IT_UPDATE, ENABLE);	// TIM4->IER |= (uint8_t)TIM4_IT;
	/* Enable TIM4 */
	TIM4_Cmd(ENABLE);	// TIM4->CR1 |= TIM4_CR1_CEN;
#else
	// set timer 4 prescale factor (typ. @16MHz: 64)
	TIM4->PSCR = (uint8_t)(T4PRESCALER);
	// set timer 4 autoreload value/period (typ. @16MHz: 250-1)
	TIM4->ARR = (uint8_t)(T4PERIOD-1);
	/* Clear TIM4 update flag by writing 0. Writing ones has no effect */
	TIM4->SR1 = (uint8_t)(~TIM4_FLAG_UPDATE);
	/* Enable update interrupt */
	TIM4->IER |= TIM4_IT_UPDATE;
	/* Enable TIM4 */
	TIM4->CR1 |= TIM4_CR1_CEN;
#endif


	// timers 1 and 2 are used for phase-correct hardware pwm
	// this is better for motors as it ensures an even waveform
	// note, however, that fast pwm mode can achieve a frequency of up
	// 8 MHz (with a 16 MHz clock) at 50% duty cycle
#ifndef NO_ANALOG_OUT
	TIM1_DeInit();	// keep this
	// actual prescaler is (n+1)
	TIM1_TimeBaseInit(63, TIM1_COUNTERMODE_UP, 255, 0);	// keep this
#ifdef USE_SPL
	TIM1_Cmd(ENABLE);

	TIM1_OC1Init(
		TIM1_OCMODE_PWM2,
		TIM1_OUTPUTSTATE_DISABLE,
		TIM1_OUTPUTNSTATE_DISABLE,
		0,
		TIM1_OCPOLARITY_HIGH,
		TIM1_OCNPOLARITY_HIGH,
		TIM1_OCIDLESTATE_SET,
		TIM1_OCNIDLESTATE_SET
	);
	TIM1_OC2Init(
		TIM1_OCMODE_PWM2,
		TIM1_OUTPUTSTATE_DISABLE,
		TIM1_OUTPUTNSTATE_DISABLE,
		0,
		TIM1_OCPOLARITY_HIGH,
		TIM1_OCNPOLARITY_HIGH,
		TIM1_OCIDLESTATE_SET,
		TIM1_OCNIDLESTATE_SET
	);
	TIM1_OC3Init(
		TIM1_OCMODE_PWM2,
		TIM1_OUTPUTSTATE_DISABLE,
		TIM1_OUTPUTNSTATE_DISABLE,
		0,
		TIM1_OCPOLARITY_HIGH,
		TIM1_OCNPOLARITY_HIGH,
		TIM1_OCIDLESTATE_SET,
		TIM1_OCNIDLESTATE_SET
	);
	TIM1_OC4Init(
		TIM1_OCMODE_PWM2,
		TIM1_OUTPUTSTATE_DISABLE,
		0,
		TIM1_OCPOLARITY_HIGH,
		TIM1_OCIDLESTATE_SET
	);
	TIM1_Cmd(ENABLE);
	TIM1_CtrlPWMOutputs(ENABLE);
#else
	TIM1->CCER1 = 0;	// channel 1 and 2 disabled
	TIM1->CCER2 = 0;	// channel 3 and 4 disabled

	TIM1->CCMR1 = TIM1_OCMODE_PWM2 | TIM1_CCMR_OCxPE;
	TIM1->CCMR2 = TIM1_OCMODE_PWM2 | TIM1_CCMR_OCxPE;
	TIM1->CCMR3 = TIM1_OCMODE_PWM2 | TIM1_CCMR_OCxPE;
	TIM1->CCMR4 = TIM1_OCMODE_PWM2 | TIM1_CCMR_OCxPE;
/* already done by TIM1_DeInit()
	TIM1->CCR1H = 0;	// write MSB first, DO NOT USE ldw instruction!
	TIM1->CCR1L = 0;
	TIM1->CCR2H = 0;	// write MSB first, DO NOT USE ldw instruction!
	TIM1->CCR2L = 0;
	TIM1->CCR3H = 0;	// write MSB first, DO NOT USE ldw instruction!
	TIM1->CCR3L = 0;
	TIM1->CCR4H = 0;	// write MSB first, DO NOT USE ldw instruction!
	TIM1->CCR4L = 0;
*/
	TIM1->CR1 = TIM1_CR1_CEN;	// TIM1_Cmd(ENABLE);
	TIM1->BKR = TIM1_BKR_MOE;	// TIM1_CtrlPWMOutputs(ENABLE);
#endif

#ifdef TIM2
	TIM2_DeInit();
	TIM2_TimeBaseInit(TIM2_PRESCALER_64, 255);

#ifdef USE_SPL
	TIM2_OC1Init(
		TIM2_OCMODE_PWM1,
		TIM2_OUTPUTSTATE_DISABLE,
		0,
		TIM2_OCPOLARITY_HIGH
	);

	TIM2_OC2Init(
		TIM2_OCMODE_PWM1,
		TIM2_OUTPUTSTATE_DISABLE,
		0,
		TIM2_OCPOLARITY_HIGH
	);

	TIM2_OC3Init(
		TIM2_OCMODE_PWM1,
		TIM2_OUTPUTSTATE_DISABLE,
		0,
		TIM2_OCPOLARITY_HIGH
	);
	TIM2_OC1PreloadConfig(ENABLE); // TIM2->CCMR1 |= (uint8_t)TIM2_CCMR_OCxPE;
	TIM2_OC2PreloadConfig(ENABLE); // TIM2->CCMR2 |= (uint8_t)TIM2_CCMR_OCxPE;
	TIM2_OC3PreloadConfig(ENABLE); // TIM2->CCMR3 |= (uint8_t)TIM2_CCMR_OCxPE;
	TIM2_Cmd(ENABLE);	// TIM2->CR1 |= (uint8_t)TIM2_CR1_CEN;
#else
	TIM2->CCER1 = 0;	// channel 1 and 2 disabled
	TIM2->CCER2 = 0;	// channel 3 and 4 disabled

	TIM2->CCMR1 = TIM2_OCMODE_PWM1 | TIM2_CCMR_OCxPE;
	TIM2->CCMR2 = TIM2_OCMODE_PWM1 | TIM2_CCMR_OCxPE;
	TIM2->CCMR3 = TIM2_OCMODE_PWM1 | TIM2_CCMR_OCxPE;

	TIM2->CR1 = TIM2_CR1_CEN;	// TIM1_Cmd(ENABLE);
#endif
#endif // #ifdef (TIM2)

#ifdef TIM3
	TIM3_DeInit();
	TIM3_TimeBaseInit(TIM3_PRESCALER_64, 255);

#ifdef USE_SPL
	TIM3_OC1Init(
		TIM3_OCMODE_PWM1,
		TIM3_OUTPUTSTATE_DISABLE,
		0,
		TIM3_OCPOLARITY_HIGH
	);

	TIM3_OC2Init(
		TIM3_OCMODE_PWM1,
		TIM3_OUTPUTSTATE_DISABLE,
		0,
		TIM3_OCPOLARITY_HIGH
	);

	TIM3_OC1PreloadConfig(ENABLE); // TIM3->CCMR1 |= (uint8_t)TIM3_CCMR_OCxPE;
	TIM3_OC2PreloadConfig(ENABLE); // TIM3->CCMR2 |= (uint8_t)TIM3_CCMR_OCxPE;
	TIM3_Cmd(ENABLE);	// TIM3->CR1 |= (uint8_t)TIM3_CR1_CEN;
#else
	TIM3->CCER1 = 0;	// channel 1 and 2 disabled
//	TIM3->CCER2 = 0;	// channel 3 disabled

	TIM3->CCMR1 = TIM3_OCMODE_PWM1 | TIM3_CCMR_OCxPE;
	TIM3->CCMR2 = TIM3_OCMODE_PWM1 | TIM3_CCMR_OCxPE;

	TIM3->CR1 = TIM3_CR1_CEN;	// TIM1_Cmd(ENABLE);
#endif
#endif // #ifdef (TIM3)
#endif // ifndef NO_ANALOG_OUT

#ifndef NO_ANALOG_IN
	
	/* De-Init ADC peripheral, sets prescaler to 2 */
	#if defined(ADC1)
		ADC1_DeInit();
		// optional:
		// set a2d prescaler so we are inside a range of 1-2 MHz
		#if F_CPU >= 18000000 // 18 MHz / 18 = 1000 KHz
			ADC1->CR1 = 7 <<4;
		#elif F_CPU >= 12000000 // 12 MHz / 12 = 1000 kHz
			ADC1->CR1 = 6 <<4;
		#elif F_CPU >= 10000000 // 10 MHz / 10 = 1000 kHz
			ADC1->CR1 = 5 <<4;
		#elif F_CPU >= 8000000 // 8 MHz / 8 = 1000 kHz
			ADC1->CR1 = 4 <<4;
		#elif F_CPU >= 6000000 // 6 MHz / 6 = 1000 kHz
			ADC1->CR1 = 4 <<4;
		#elif F_CPU >= 4000000 // 4 MHz / 4 = 1000 kHz
			ADC1->CR1 = 3 <<4;
		#elif F_CPU >= 3000000 // 3 MHz / 3 = 1000 kHz
			ADC1->CR1 = 2 <<4;
		#elif F_CPU >= 2000000 // 2 MHz / 2 = 1000 kHz
			ADC1->CR1 = 1 <<4;
		//#else // minimum prescaler is 2, already set by ADC1_DeInit();
		//	ADC1->CR1 = 0 <<4;
		#endif

	#elif defined(ADC2)
		ADC2_DeInit();
		// optional:
		// set a2d prescaler so we are inside a range of 1-2 MHz
		#if F_CPU >= 18000000 // 18 MHz / 18 = 1000 KHz
			ADC2->CR1 = 7 <<4;
		#elif F_CPU >= 12000000 // 12 MHz / 12 = 1000 kHz
			ADC2->CR1 = 6 <<4;
		#elif F_CPU >= 10000000 // 10 MHz / 10 = 1000 kHz
			ADC2->CR1 = 5 <<4;
		#elif F_CPU >= 8000000 // 8 MHz / 8 = 1000 kHz
			ADC2->CR1 = 4 <<4;
		#elif F_CPU >= 6000000 // 6 MHz / 6 = 1000 kHz
			ADC2->CR1 = 4 <<4;
		#elif F_CPU >= 4000000 // 4 MHz / 4 = 1000 kHz
			ADC2->CR1 = 3 <<4;
		#elif F_CPU >= 3000000 // 3 MHz / 3 = 1000 kHz
			ADC2->CR1 = 2 <<4;
		#elif F_CPU >= 2000000 // 2 MHz / 2 = 1000 kHz
			ADC2->CR1 = 1 <<4;
		//#else // minimum prescaler is 2, already set by ADC1_DeInit();
		//	ADC2->CR1 = 0 <<4;
		#endif

	#endif // ADC2

#endif // ifndef NO_ANALOG_IN

	// this needs to be called before setup() or some functions won't
	// work there
	enableInterrupts();
}
