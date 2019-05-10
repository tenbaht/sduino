#ifndef _SERIAL_H_
#define _SERIAL_H_

/*
 * not more than a convininience module to smoothen the transition from
 * real Arduino to sduino syntax for existing sketches.
 *
 * All it does is to include HardwareSerial.h, so it doesn't matter if you
 * include HardwareSerial or Serial into your code.
 *
 */

#include <Arduino.h>
#include <HardwareSerial.h>
#include <Print.h>


// Public Methods //////////////////////////////////////////////////////////////

// Alias Definitions for a more Arduino-like look ////////////////////////////

/*
 * all empty.
 *
 * so far, the alias definitions are done in HardwareSerial and not here.
 * This file is supposed to be a kind of intermediate switch to choose the
 * serial module depending on the configured CPU.
 *
 */


#endif
