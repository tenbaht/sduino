#ifndef muBoard_h
#define muBoard_h

#include <stdio.h>
#include <string.h>
//#include <inttypes.h>
#include "Arduino.h"
#include "beep.h"

// HW version v2.2 w/o CAN, or v2.3 with CAN
//#define HW_VERSION 22
#define HW_VERSION 23

/// macro for HV-bridge control (BRIDGE_1-4 correspond to digital pins 26-29; True = ON = output high)
#define set_bridge(num,state)		digitalWrite(25+num, state)

/// macro for LS-switch control (LS_1-3 correspond to digital pins 23-25; True = ON = output low) 
#define set_lowside(num,state)		digitalWrite(22+num, state)


#ifdef __cplusplus

// C++ interface for use with regular gcc compiler

#else

  /// init muBoard peripherals
  void initVariant();

#endif /* c interface */

#endif /* muBoard_h */
