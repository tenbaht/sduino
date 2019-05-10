/*
  wiring_digital.c - digital input and output functions
  Part of Arduino - http://www.arduino.cc/

  Copyright (c) 2005-2006 David A. Mellis

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General
  Public License along with this library; if not, write to the
  Free Software Foundation, Inc., 59 Temple Place, Suite 330,
  Boston, MA  02111-1307  USA

  Modified 28 September 2010 by Mark Sproul
*/

#include "wiring_private.h"
#include "pins_arduino.h"



/* this typedef is a SDCC wordaround.
 * Defining this a type instead of defining an array of (unsigned char *)
 * keeps SDCC from storing the array in the xinit segment and copy it to RAM
 * at runtime.
 */
typedef unsigned char *uc_p;

extern const uc_p ccmrx[NUM_TIMERS];
extern const uc_p ccerx[NUM_TIMERS];
extern const unsigned char DISABLE_TIMER_OUTP_MASK[NUM_TIMERS];
void turnOffPWM(uint8_t timer);


