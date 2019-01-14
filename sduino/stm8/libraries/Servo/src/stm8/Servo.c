/*
 Servo.cpp - Interrupt driven Servo library for Arduino using 16 bit timers- Version 2
 Copyright (c) 2009 Michael Margolis.  All right reserved.

 This library is free software; you can redistribute it and/or
 modify it under the terms of the GNU Lesser General Public
 License as published by the Free Software Foundation; either
 version 2.1 of the License, or (at your option) any later version.

 This library is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 Lesser General Public License for more details.

 You should have received a copy of the GNU Lesser General Public
 License along with this library; if not, write to the Free Software
 Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */

#if defined(ARDUINO_ARCH_STM8)

//#include <avr/interrupt.h>
#include <Arduino.h>
#include "Servo.h"

#define usToTicks(_us)    (( clockCyclesPerMicrosecond()* _us) / 8)     // converts microseconds to tick (assumes prescale of 8)  // 12 Aug 2009
#define ticksToUs(_ticks) (( (unsigned)_ticks * 8)/ clockCyclesPerMicrosecond() ) // converts from ticks back to microseconds


#define TRIM_DURATION       2                               // compensation ticks to trim adjust for digitalWrite delays // 12 August 2009

//#define NBR_TIMERS        (MAX_SERVOS / SERVOS_PER_TIMER)

typedef struct  {
  uint8_t nbr        :6 ;             // a pin number from 0 to 63
  uint8_t isActive   :1 ;             // true if this channel is enabled, pin not pulsed if false 
} ServoPin_t   ;  

typedef struct {
  ServoPin_t Pin;
  volatile unsigned int ticks;
  int8_t min;                       // minimum is this value times 4 added to MIN_PULSE_WIDTH    
  int8_t max;                       // maximum is this value times 4 added to MAX_PULSE_WIDTH   
} servo_t;

static servo_t servos[MAX_SERVOS];                          // static array of servo structures
static volatile int8_t Channel[_Nbr_16timers ];             // counter for the servo being pulsed for each timer (or -1 if refresh interval)

static uint8_t ServoCount = 0;                                     // the total number of attached servos


// convenience macros
#define SERVO_INDEX_TO_TIMER(_servo_nbr) ((timer16_Sequence_t)(_servo_nbr / SERVOS_PER_TIMER)) // returns the timer controlling this servo
#define SERVO_INDEX_TO_CHANNEL(_servo_nbr) (_servo_nbr % SERVOS_PER_TIMER)       // returns the index of the servo on this timer
#define SERVO_INDEX(_timer,_channel)  ((_timer*SERVOS_PER_TIMER) + _channel)     // macro to access servo index by timer and channel
#define SERVO(_timer,_channel)  (servos[SERVO_INDEX(_timer,_channel)])            // macro to access servo class by timer and channel

#define SERVO_MIN(_channel) (MIN_PULSE_WIDTH - servos[_channel].min * 4)  // minimum value in uS for this servo
#define SERVO_MAX(_channel) (MAX_PULSE_WIDTH - servos[_channel].max * 4)  // maximum value in uS for this servo

/************ static functions common to all instances ***********************/

uint16_t irqcnt;	// debugging only

static void handle_interrupts(timer16_Sequence_t timer, volatile uint8_t *TCNTn, volatile uint8_t* OCRnA)
{
  // stm8
  unsigned tcnt;
  unsigned ccr;

  if( Channel[timer] < 0 ) {
    *TCNTn = 0; // channel set to -1 indicated that refresh interval completed so reset the timer
    *(TCNTn+1) = 0;	// write MSB first
  } else {
    if( SERVO_INDEX(timer,Channel[timer]) < ServoCount && SERVO(timer,Channel[timer]).Pin.isActive == true )
      digitalWrite( SERVO(timer,Channel[timer]).Pin.nbr,LOW); // pulse this channel low if activated
  }
  Channel[timer]++;    // increment to the next channel

  tcnt  = *TCNTn << 8;	// read MSB first, do not use ldw instruction
  tcnt |= *(TCNTn+1);	// read LSB
  if( SERVO_INDEX(timer,Channel[timer]) < ServoCount && Channel[timer] < SERVOS_PER_TIMER) {
    ccr = tcnt + SERVO(timer,Channel[timer]).ticks;
    if(SERVO(timer,Channel[timer]).Pin.isActive == true)     // check if activated
      digitalWrite( SERVO(timer,Channel[timer]).Pin.nbr,HIGH); // its an active channel so pulse it high
  }
  else {
    // finished all channels so wait for the refresh period to expire before starting over
    if( (tcnt + 500) < usToTicks(REFRESH_INTERVAL) )  // allow a few ticks to ensure the next OCR1A not missed
      ccr = (unsigned int)usToTicks(REFRESH_INTERVAL);
    else
      ccr = tcnt + 500;  // at least REFRESH_INTERVAL has elapsed
    Channel[timer] = -1; // this will get incremented at the end of the refresh period to start again at the first channel
  }
  *OCRnA     = ccr >> 8;	// write MSB first, do not use ldw instruction
  *(OCRnA+1) = ccr & 0xff;	// write LSB
}



#ifndef WIRING // Wiring pre-defines signal handlers so don't define any if compiling for the Wiring platform
// Interrupt handlers for Arduino
#if defined(_useTimer1)
INTERRUPT_HANDLER(TIM1_CAP_COM_IRQHandler, ITC_IRQ_TIM1_CAPCOM)
//void t1ccservice(void)
{
  TIM1->SR1	= 0;		// clear any pending interrupts;
  irqcnt++;	// debug only
  handle_interrupts(_timer1, &TIM1->CNTRH, &TIM1->CCR1H);
}
#endif

#if defined(_useTimer2)
INTERRUPT_HANDLER(TIM2_CAP_COM_IRQHandler, ITC_IRQ_TIM2_CAPCOM)
{
  TIM2->SR1	= 0;		// clear any pending interrupts;
  handle_interrupts(_timer2, &TIM2->CNTRH, &TIM2->CCR1H);
}
#endif

#if defined(_useTimer3)
INTERRUPT_HANDLER(TIM3_CAP_COM_IRQHandler, ITC_IRQ_TIM3_CAPCOM)
{
  TIM3->SR1	= 0;		// clear any pending interrupts;
  handle_interrupts(_timer3, &TIM3->CNTRH, &TIM3->CCR1H);
}
#endif

#if defined(_useTimer5)
INTERRUPT_HANDLER(TIM5_CAP_COM_IRQHandler, ITC_IRQ_TIM5_CAPCOM)
{
  TIM3->SR1	= 0;		// clear any pending interrupts;
  handle_interrupts(_timer5, &TIM5->CNTRH, &TIM5->CCR1H);
}
#endif

#elif defined WIRING
// Interrupt handlers for Wiring
#if defined(_useTimer1)
void Timer1Service()
{
  TIM1->SR1	= 0;		// clear any pending interrupts;
  handle_interrupts(_timer1, &TIM1->CNTRH, &TIM1->CCR1H);
}
#endif
#if defined(_useTimer2)
void Timer2Service()
{
  TIM2->SR1	= 0;		// clear any pending interrupts;
  handle_interrupts(_timer2, &TIM2->CNTRH, &TIM2->CCR1H);
}
#endif
#if defined(_useTimer3)
void Timer3Service()
{
  TIM3->SR1	= 0;		// clear any pending interrupts;
  handle_interrupts(_timer3, &TIM3->CNTRH, &TIM3->CCR1H);
}
#endif
#endif


static void initISR(timer16_Sequence_t timer)
{
#if defined (_useTimer1)
  if(timer == _timer1) {
    // do the AND to avoid a compiler warning.
    attachInterrupt(INT_TIM1_CAPCOM & 0xff, Timer1Service, 0);
    TIM1_TimeBaseInit(7, TIM1_COUNTERMODE_UP, 0xffff, 0);     // keep this
/* all included in TIM1_TimeBaseInit()
    TIM1->CR1	= 0;		// disable counter, mode upcount, no preload
    TIM1->ARRH	= 0xff;		// auto reload register
    TIM1->ARRL	= 0xff;
    TIM1->PSCRH	= 0;		// set prescaler of 8 (=7+1)
    TIM1->PSCRL	= 7;

//    TIM1->CR1	= 0;		// disable counter, mode upcount
    TIM1->RCR	= 0;		// *Set the Repetition Counter value
*/

    TIM1->CCER1	= 0;		// disable PWM outputs
//    TIM1->CCER2	= 0;
    TIM1->CCMR1	= 0;		// OC1 does not affect the pins

    // initalize the capcom register for the first event
    TIM1->CCR1H = usToTicks(REFRESH_INTERVAL) >> 8;	// ok
    TIM1->CCR1L = usToTicks(REFRESH_INTERVAL) & 0xff;

    // very important: trigger an update event to change the prescaler
    TIM1->EGR	= 1;		// trigger an update event to change the prescaler

//    attach_irq(INT_TIM1_CC, &t1ccservice);
    TIM1->SR1	= 0;		// clear any pending interrupts;
    TIM1->IER	= TIM1_IER_CC1IE;// enable the output compare interrupt
    TIM1->CR1	= TIM1_CR1_CEN;	// enable counter, mode upcount

/*
    TIM1->CR1	= 0;		// disable counter, mode upcount
    TIM1->CNTRH	= 0;		// clear the timer count, MSB first (not critcal)
    TIM1->CNTRL	= 0;		// clear the timer count
    TIM1->PSCRH	= 0;		// set prescaler of 8
    TIM1->PSCRL	= 8;
    TIM1->ARRH	= 0xff;		// auto reload register
    TIM1->ARRL	= 0xff;
    TIM1->RCR	= 0;		// *Set the Repetition Counter value
    TIM1->CR1	= TIM1_CR1_CEN;	// enable counter, mode upcount
    TIM1->SR1	= 0;		// clear any pending interrupts;
    TIM1->IER	|= TIM1_IER_CC1IE;// enable the output compare interrupt
*/
  }
#endif

/*FIXME
#if defined (_useTimer3)
  if(timer == _timer3) {
    TCCR3A = 0;             // normal counting mode
    TCCR3B = _BV(CS31);     // set prescaler of 8
    TCNT3 = 0;              // clear the timer count
#if defined(__AVR_ATmega128__)
    TIFR |= _BV(OCF3A);     // clear any pending interrupts;
	ETIMSK |= _BV(OCIE3A);  // enable the output compare interrupt
#else
    TIFR3 = _BV(OCF3A);     // clear any pending interrupts;
    TIMSK3 =  _BV(OCIE3A) ; // enable the output compare interrupt
#endif
#if defined(WIRING)
    timerAttach(TIMER3OUTCOMPAREA_INT, Timer3Service);  // for Wiring platform only
#endif
  }
#endif

#if defined (_useTimer4)
  if(timer == _timer4) {
    TCCR4A = 0;             // normal counting mode
    TCCR4B = _BV(CS41);     // set prescaler of 8
    TCNT4 = 0;              // clear the timer count
    TIFR4 = _BV(OCF4A);     // clear any pending interrupts;
    TIMSK4 =  _BV(OCIE4A) ; // enable the output compare interrupt
  }
#endif

#if defined (_useTimer5)
  if(timer == _timer5) {
    TCCR5A = 0;             // normal counting mode
    TCCR5B = _BV(CS51);     // set prescaler of 8
    TCNT5 = 0;              // clear the timer count
    TIFR5 = _BV(OCF5A);     // clear any pending interrupts;
    TIMSK5 =  _BV(OCIE5A) ; // enable the output compare interrupt
  }
#endif
*/
}

static void finISR(timer16_Sequence_t timer)
{
/*FIXME
    //disable use of the given timer
#if defined WIRING   // Wiring
  if(timer == _timer1) {
    #if defined(__AVR_ATmega1281__)||defined(__AVR_ATmega2561__)
    TIMSK1 &=  ~_BV(OCIE1A) ;  // disable timer 1 output compare interrupt
    #else
    TIMSK &=  ~_BV(OCIE1A) ;  // disable timer 1 output compare interrupt
    #endif
    timerDetach(TIMER1OUTCOMPAREA_INT);
  }
  else if(timer == _timer3) {
    #if defined(__AVR_ATmega1281__)||defined(__AVR_ATmega2561__)
    TIMSK3 &= ~_BV(OCIE3A);    // disable the timer3 output compare A interrupt
    #else
    ETIMSK &= ~_BV(OCIE3A);    // disable the timer3 output compare A interrupt
    #endif
    timerDetach(TIMER3OUTCOMPAREA_INT);
  }
#else
    //For arduino - in future: call here to a currently undefined function to reset the timer
#endif
*/
}

static boolean isTimerActive(timer16_Sequence_t timer)
{
  // returns true if any servo is active on this timer
  for(uint8_t channel=0; channel < SERVOS_PER_TIMER; channel++) {
    if(SERVO(timer,channel).Pin.isActive == true)
      return true;
  }
  return false;
}


/****************** end of static functions ******************************/

uint8_t Servo_attach(int pin)
{
  return Servo_attach_minmax(pin, MIN_PULSE_WIDTH, MAX_PULSE_WIDTH);
}

uint8_t Servo_attach_minmax(int pin, int min, int max)
{
  timer16_Sequence_t timer;
  uint8_t servoIndex;

  if( ServoCount < MAX_SERVOS) {
    servoIndex = ServoCount++;                    // assign a servo index to this instance
    servos[servoIndex].ticks = usToTicks(DEFAULT_PULSE_WIDTH);   // store default values  - 12 Aug 2009

    pinMode( pin, OUTPUT) ;                                   // set servo pin to output
    servos[servoIndex].Pin.nbr = pin;
    // todo min/max check: abs(min - MIN_PULSE_WIDTH) /4 < 128
    servos[servoIndex].min  = (MIN_PULSE_WIDTH - min)/4; //resolution of min/max is 4 uS
    servos[servoIndex].max  = (MAX_PULSE_WIDTH - max)/4;
    // initialize the timer if it has not already been initialized
    timer = SERVO_INDEX_TO_TIMER(servoIndex);
    if(isTimerActive(timer) == false)
      initISR(timer);
    servos[servoIndex].Pin.isActive = true;  // this must be set after the check for isTimerActive
  }
  else {
    return INVALID_SERVO ;  // too many servos
  }
  return servoIndex ;
}

void Servo_detach(byte channel)
{
  timer16_Sequence_t timer;

  servos[channel].Pin.isActive = false;
  timer = SERVO_INDEX_TO_TIMER(channel);
  if(isTimerActive(timer) == false) {
    finISR(timer);
  }
}

void Servo_write(byte channel, int value)
{
  if(value < MIN_PULSE_WIDTH)
  {  // treat values less than 544 as angles in degrees (valid values in microseconds are handled as microseconds)
    if(value < 0) value = 0;
    if(value > 180) value = 180;
    value = map(value, 0, 180, SERVO_MIN(channel),  SERVO_MAX(channel));
  }
  Servo_writeMicroseconds(channel, value);
}

void Servo_writeMicroseconds(byte channel, int value)
{
  // calculate and store the values for the given channel
  if( (channel < MAX_SERVOS) )   // ensure channel is valid
  {
    if( value < SERVO_MIN(channel) )          // ensure pulse width is valid
      value = SERVO_MIN(channel);
    else if( value > SERVO_MAX(channel) )
      value = SERVO_MAX(channel);

    value = value - TRIM_DURATION;
    value = usToTicks(value);  // convert to ticks after compensating for interrupt overhead - 12 Aug 2009

    BEGIN_CRITICAL
    servos[channel].ticks = value;
    END_CRITICAL
  }
}

int Servo_read(byte channel) // return the value as degrees
{
  return  map( Servo_readMicroseconds(channel)+1, SERVO_MIN(channel), SERVO_MAX(channel), 0, 180);
}

int Servo_readMicroseconds(byte channel)
{
  unsigned int pulsewidth;
  if( servos[channel].Pin.isActive == true )
    pulsewidth = ticksToUs(servos[channel].ticks)  + TRIM_DURATION ;   // 12 aug 2009
  else
    pulsewidth  = 0;

  return pulsewidth;
}

bool Servo_attached(byte channel)
{
  return servos[channel].Pin.isActive ;
}

#endif // ARDUINO_ARCH_AVR

