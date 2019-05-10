/*
  Servo.h - Interrupt driven Servo library for Arduino using 16 bit timers- Version 2
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

/* 
  A servo is activated by creating an instance of the Servo class passing 
  the desired pin to the attach() method.
  The servos are pulsed in the background using the value most recently 
  written using the write() method.

  Note that analogWrite of PWM on pins associated with the timer are 
  disabled when the first servo is attached.
  Timers are seized as needed in groups of 12 servos - 24 servos use two 
  timers, 48 servos will use four.
  The sequence used to sieze timers is defined in timers.h

  The methods are:

    Servo - Class for manipulating servo motors connected to Arduino pins.

    attach(pin )  - Attaches a servo motor to an i/o pin.
    attach(pin, min, max  ) - Attaches to a pin setting min and max values in microseconds
    default min is 544, max is 2400  
 
    write()     - Sets the servo angle in degrees.  (invalid angle that is valid as pulse in microseconds is treated as microseconds)
    writeMicroseconds() - Sets the servo pulse width in microseconds 
    read()      - Gets the last written servo pulse width as an angle between 0 and 180. 
    readMicroseconds()   - Gets the last written servo pulse width in microseconds. (was read_us() in first release)
    attached()  - Returns true if there is a servo attached. 
    detach()    - Stops an attached servos from pulsing its i/o pin. 
 */

#ifndef Servo_h
#define Servo_h

//#include <inttypes.h>

/* 
 * Defines for 16 bit timers used with  Servo library 
 *
 * If _useTimerX is defined then TimerX is a 16 bit timer on the current board
 * timer16_Sequence_t enumerates the sequence that the timers should be allocated
 * _Nbr_16timers indicates how many 16 bit timers are available.
 */

// Architecture specific include
#if defined(ARDUINO_ARCH_AVR)
#include "avr/ServoTimers.h"
#elif defined(ARDUINO_ARCH_SAM)
#include "sam/ServoTimers.h"
#elif defined(ARDUINO_ARCH_SAMD)
#include "samd/ServoTimers.h"
#elif defined(ARDUINO_ARCH_STM8)
#include "stm8/ServoTimers.h"
#else
#error "This library only supports boards with an AVR, SAM, SAMD or STM8 processor."
#endif

#define Servo_VERSION           2     // software version of this library

#define MIN_PULSE_WIDTH       544     // the shortest pulse sent to a servo  
#define MAX_PULSE_WIDTH      2400     // the longest pulse sent to a servo 
#define DEFAULT_PULSE_WIDTH  1500     // default pulse width when servo is attached
#define REFRESH_INTERVAL    20000     // minumim time to refresh servos in microseconds 

#define SERVOS_PER_TIMER       12     // the maximum number of servos controlled by one timer 
#define MAX_SERVOS   (_Nbr_16timers  * SERVOS_PER_TIMER)

#define INVALID_SERVO         255     // flag indicating an invalid servo index

#ifdef __cplusplus

// C++ interface for use with regular gcc compiler

typedef struct  {
  uint8_t nbr        :6 ;             // a pin number from 0 to 63
  uint8_t isActive   :1 ;             // true if this channel is enabled, pin not pulsed if false 
} ServoPin_t   ;  

typedef struct {
  ServoPin_t Pin;
  volatile unsigned int ticks;
} servo_t;


class Servo
{
public:
  Servo();
  uint8_t attach(int pin);           // attach the given pin to the next free channel, sets pinMode, returns channel number or 0 if failure
  uint8_t attach(int pin, int min, int max); // as above but also sets min and max values for writes. 
  void detach();
  void write(int value);             // if value is < 200 its treated as an angle, otherwise as pulse width in microseconds 
  void writeMicroseconds(int value); // Write pulse width in microseconds 
  int read();                        // returns current pulse width as an angle between 0 and 180 degrees
  int readMicroseconds();            // returns current pulse width in microseconds for this servo (was read_us() in first release)
  bool attached();                   // return true if this servo is attached, otherwise false 
private:
   uint8_t servoIndex;               // index into the channel data for this servo
   int8_t min;                       // minimum is this value times 4 added to MIN_PULSE_WIDTH    
   int8_t max;                       // maximum is this value times 4 added to MAX_PULSE_WIDTH   
};
#else

// plain C interface for use with SDCC

typedef unsigned char Servo;		// used as a class instance aquivalent
/* does not need to be exported
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
*/

uint8_t Servo_attach(int pin);           // attach the given pin to the next free channel, sets pinMode, returns channel number or 0 if failure
uint8_t Servo_attach_minmax(int pin, int min, int max); // as above but also sets min and max values for writes. 
void Servo_detach(byte channel);
void Servo_write(byte channel, int value); // if value is < 200 its treated as an angle, otherwise as pulse width in microseconds 
void Servo_writeMicroseconds(byte channel, int value); // Write pulse width in microseconds 
int Servo_read(byte channel);              // returns current pulse width as an angle between 0 and 180 degrees
int Servo_readMicroseconds(byte channel);  // returns current pulse width in microseconds for this servo (was read_us() in first release)
bool Servo_attached(byte channel);         // return true if this servo is attached, otherwise false 


// Pseudo-OO interface: Plain C disguised as almost-C++, thanks to X-Macros
#include <xmacro.h>

/* usage:
 * Instantiation:
 *   Typically as a global definition. Has to be at the placed in the source
 *   file before any of the "methods" can be used.
 *     Servo(instancename);
 * Constructors (typically in the setup() function):
 *   Different constructors to emulate the polymorph class constructor
 *     instancename_attach(pin);
 *     instancename_attach_minmax(pin, min, max);
 * Methods:
 *     instancename_detach();
 *     instancename_write(val);
 *     instancename_writeMicroseconds(val);
 *     int instancename_read();
 *     int instancename_readMicroseconds();
 *     bool instancename_attached();
 */

// The instantiation macro *must* be first in the list to allow for a
// extern declaration if the global "object" is used in different source code
// modules.
//
// And it is duplicated as an external declaration at the end of the list to
// avoid a compiler syntax error with the following ';'

// The instantiation macro *must* be the last in the list as it is the only
// macro that does not result in a syntax error with the following ';'
#define Servo(instance) \
	XInstanciation	(Servo,instance); \
	XConstructor1	(Servo,instance,attach,int) \
	XConstructor3	(Servo,instance,attach_minmax,int,int,int) \
	XMethod0	(Servo,instance,detach) \
	XMethod1	(Servo,instance,write,int) \
	XMethod1	(Servo,instance,writeMicroseconds,int) \
	XMethod0return	(Servo,instance,int,read) \
	XMethod0return	(Servo,instance,int,readMicroseconds) \
	XMethod0return	(Servo,instance,bool,attached) \
	extern XInstanciation	(Servo,instance)


#endif

#endif
