/*
  I2C.h - I2C library
  Based on the I2C library for Arduino by Wayne Truchsess.
  Ported to C for the STM8S by Michael Mayer.
  Rev 5.0s1 - ported to C for the STM8S103F3 using SDCC

  Copyright (c) 2011-2012 Wayne Truchsess.  All right reserved.
  Rev 5.0 - January 24th, 2012
          - Removed the use of interrupts completely from the library
            so TWI state changes are now polled. 
          - Added calls to lockup() function in most functions 
            to combat arbitration problems 
          - Fixed scan() procedure which left timeouts enabled 
            and set to 80msec after exiting procedure
          - Changed scan() address range back to 0 - 0x7F
          - Removed all Wire legacy functions from library
          - A big thanks to Richard Baldwin for all the testing
            and feedback with debugging bus lockups!
  Rev 4.0 - January 14th, 2012
          - Updated to make compatible with 8MHz clock frequency
  Rev 3.0 - January 9th, 2012
          - Modified library to be compatible with Arduino 1.0
          - Changed argument type from boolean to uint8_t in pullUp(), 
            setSpeed() and receiveByte() functions for 1.0 compatability
          - Modified return values for timeout feature to report
            back where in the transmission the timeout occured.
          - added function scan() to perform a bus scan to find devices
            attached to the I2C bus.  Similar to work done by Todbot
            and Nick Gammon
  Rev 2.0 - September 19th, 2011
          - Added support for timeout function to prevent 
            and recover from bus lockup (thanks to PaulS
            and CrossRoads on the Arduino forum)
          - Changed return type for stop() from void to
            uint8_t to handle timeOut function 
  Rev 1.0 - August 8th, 2011
  
  This is a modified version of the Arduino Wire/TWI 
  library.  Functions were rewritten to provide more functionality
  and also the use of Repeated Start.  Some I2C devices will not
  function correctly without the use of a Repeated Start.  The 
  initial version of this library only supports the Master.


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

#if(ARDUINO >= 100)
#include <Arduino.h>
#else
#include <WProgram.h>
#endif

//#include <inttypes.h>

#ifndef I2C_h
#define I2C_h


/* TWI status codes for AVR
#define START           0x08
#define REPEATED_START  0x10
#define MT_SLA_ACK	0x18
#define MT_SLA_NACK	0x20
#define MT_DATA_ACK     0x28
#define MT_DATA_NACK    0x30
#define MR_SLA_ACK	0x40
#define MR_SLA_NACK	0x48
#define MR_DATA_ACK     0x50
#define MR_DATA_NACK    0x58
#define LOST_ARBTRTN    0x38
#define TWI_STATUS      (TWSR & 0xF8)
*/
#define LOST_ARBTRTN    0x38
#define SLA_W(address)  (address << 1)
#define SLA_R(address)  ((address << 1) + 0x01)
#define cbi(sfr, bit)   (_SFR_BYTE(sfr) &= ~_BV(bit))
#define sbi(sfr, bit)   (_SFR_BYTE(sfr) |= _BV(bit))

#define MAX_BUFFER_SIZE 32




    void I2C_begin();
    void I2C_end();
    void I2C_timeOut(uint16_t);
    void I2C_setSpeed(uint8_t); 
    void I2C_pullup(uint8_t);
    void I2C_scan();
    uint8_t I2C_available();
    uint8_t I2C_receive();
    uint8_t I2C_write(uint8_t, uint8_t);
    uint8_t I2C_write_c(uint8_t, uint8_t, uint8_t);
    uint8_t I2C_write_s(uint8_t, uint8_t, char*);
    uint8_t I2C_write_sn(uint8_t, uint8_t, uint8_t*, uint8_t);
    uint8_t I2C_read(uint8_t, uint8_t);
    uint8_t I2C_read_c(uint8_t, uint8_t, uint8_t);
    uint8_t I2C_read_s(uint8_t, uint8_t, uint8_t*);
    uint8_t I2C_read_sn(uint8_t, uint8_t, uint8_t, uint8_t*);


//  private:
/*
    uint8_t I2C_start(void);
    uint8_t I2C_sendAddress(uint8_t);
    uint8_t I2C_sendByte(uint8_t);
    uint8_t I2C_receiveByte(uint8_t);
    uint8_t I2C_stop(void);
    void I2C_lockUp(void);
*/
// not sure if these really need to be public:
    extern uint8_t returnStatus;
    extern uint8_t nack;
    extern uint8_t data[MAX_BUFFER_SIZE];

#endif
