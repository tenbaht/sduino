/*
  I2C.h - I2C library
  It started as a port of the I2C library for Arduino by Wayne Truchsess,
  but since the I2C device of the STM8 is programmed so fundamentally different
  then I2C for the AVR it ended up to be a complete rewrite from scratch.
  Only the external API is unchanged.

  Re-written in C for the STM8S by Michael Mayer.

  Copyright (c) 2017-2018 Michael Mayer.  All right reserved.
  Rev 6.0 - Novemver 8th, 2018
          - added support for Master Receiver mode
          - complete rewrite of the library
          - more regular and logical function naming
  Rev 5.0s1 - ported to C for the STM8S103F3 using SDCC
          - only master transmitt mode supported, no read access

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

/* TWI status codes */
/*
 * return values for functions that use the timeOut feature 
 * will now return at what point in the transmission the timeout
 * occurred. Looking at a full communication sequence between a 
 * master and slave (transmit data and then readback data) there
 * a total of 7 points in the sequence where a timeout can occur.
 *
 * These are listed below and correspond to the returned value:
 *
 * 0 - Function executed with no errors
 * 1 - Waiting for successful completion of a Start bit
 * 2 - Waiting for ACK/NACK while addressing slave in transmit mode (MT)
 * 3 - Waiting for ACK/NACK while sending data to the slave
 * 4 - Waiting for successful completion of a Repeated Start
 * 5 - Waiting for ACK/NACK while addressing slave in receiver mode (MR)
 * 6 - Waiting for ACK/NACK while receiving data from the slave
 * 7 - Waiting for successful completion of the Stop bit
 * 0x38	- arbitration lost
 */

#define LOST_ARBTRTN    0x38

void I2C_begin();
void I2C_end();
void I2C_timeOut(uint16_t);
void I2C_setSpeed(uint8_t);
void I2C_pullup(uint8_t);
void I2C_scan();
uint8_t I2C_available();
uint8_t I2C_receive();
uint8_t I2C_write(uint8_t, uint8_t);
uint8_t I2C_write_reg(uint8_t, uint8_t, uint8_t);
uint8_t I2C_write_s(uint8_t, uint8_t, char *);
uint8_t I2C_write_sn(uint8_t, uint8_t, uint8_t *, uint8_t);
uint8_t I2C_read(uint8_t address, uint8_t numberBytes);
uint8_t I2C_read_reg(uint8_t address, uint8_t registerAddress,
		     uint8_t numberBytes);
uint8_t I2C_readbuf(uint8_t address, uint8_t numberBytes, uint8_t * dataBuffer);
uint8_t I2C_readbuf_reg(uint8_t address, uint8_t registerAddress,
			uint8_t numberBytes, uint8_t * dataBuffer);

// not sure if these really need to be public:
//    extern uint8_t returnStatus;
//    extern uint8_t data[MAX_BUFFER_SIZE];

#endif
