/*
  I2C.c - I2C library
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
#include "I2C.h"
#include "Serial.h"

#define SLA_W(address)  (address << 1)
#define SLA_R(address)  ((address << 1) + 0x01)
#define cbi(sfr, bit)   (_SFR_BYTE(sfr) &= ~_BV(bit))
#define sbi(sfr, bit)   (_SFR_BYTE(sfr) |= _BV(bit))

#define MAX_BUFFER_SIZE 32

uint8_t returnStatus;
uint8_t data[MAX_BUFFER_SIZE];

static uint8_t bytesAvailable;
static uint8_t bufferIndex;
static uint8_t totalBytes;
static uint16_t timeOutDelay;

/* --- private functions ------------------------------------------------- */
static uint8_t sendAddress(uint8_t, uint8_t);
static uint8_t sendByte(uint8_t);
static uint8_t receiveByte(void);
static uint8_t stop(void);
static void lockUp(void);

#define MODE_WRITE 4

#define SEND_ADDRESS_W(ADDR) \
  returnStatus = sendAddress(SLA_W(ADDR),MODE_WRITE); \
  if(returnStatus) \
  { \
    if(returnStatus == 1){return(2);} \
    return(returnStatus); \
  }

#define SEND_BYTE(VAL) \
  returnStatus = sendByte(VAL); \
  if(returnStatus) \
  { \
    if(returnStatus == 1){return(3);} \
    return(returnStatus); \
  }

// wait while the condition is still true (wait for a bit to become zero)
#define TIMEOUT_WAIT_FOR_ZERO(CONDITION,ERROR) \
	while (CONDITION) 	/* wait while the condition is still true */ \
	{ \
          if(!timeOutDelay){continue;} \
          if((((uint16_t)millis()) - startingTime) >= timeOutDelay) \
          { \
            lockUp(); \
            return(ERROR); 		/* return the appropriate error code */ \
          } \
        }

// wait while the condition is not true (wait for a bit to become one)
#define TIMEOUT_WAIT_FOR_ONE(CONDITION,ERROR) TIMEOUT_WAIT_FOR_ZERO(!(CONDITION), ERROR)

/* --- public methods ---------------------------------------------------- */

void I2C_begin()
{
	bytesAvailable = 0;
	bufferIndex = 0;
	totalBytes = 0;
	timeOutDelay = 10;	// set default time out
	I2C_setSpeed(0);	// initialize for standard speed (100kHz)
}

void I2C_end()
{
	I2C->CR1 = 0;
}

void I2C_timeOut(uint16_t _timeOut)
{
	timeOutDelay = _timeOut;
}

void I2C_setSpeed(uint8_t _fast)
{
	/* it is easier to use the full initialization function */
	I2C_Init(_fast ? I2C_MAX_FAST_FREQ : I2C_MAX_STANDARD_FREQ,	// 100k/400k
		 0xA0,		// OwnAddress, egal
		 I2C_DUTYCYCLE_2,	// 0x00
		 I2C_ACK_CURR,	// 0x01
		 I2C_ADDMODE_7BIT,	// 0x00
		 16		// InputClockFreqencyMhz
	    );

}

#if 0
void I2C_pullup(uint8_t activate)
{
	if (activate) {
#if defined(__AVR_ATmega168__) || defined(__AVR_ATmega8__) || defined(__AVR_ATmega328P__)
		// activate internal pull-ups for twi
		// as per note from atmega8 manual pg167
		sbi(PORTC, 4);
		sbi(PORTC, 5);
#else
		// activate internal pull-ups for twi
		// as per note from atmega128 manual pg204
		sbi(PORTD, 0);
		sbi(PORTD, 1);
#endif
	} else {
#if defined(__AVR_ATmega168__) || defined(__AVR_ATmega8__) || defined(__AVR_ATmega328P__)
		// deactivate internal pull-ups for twi
		// as per note from atmega8 manual pg167
		cbi(PORTC, 4);
		cbi(PORTC, 5);
#else
		// deactivate internal pull-ups for twi
		// as per note from atmega128 manual pg204
		cbi(PORTD, 0);
		cbi(PORTD, 1);
#endif
	}
}
#endif

void I2C_scan()
{
	uint16_t tempTime = timeOutDelay;
	uint8_t totalDevicesFound = 0;
	timeOutDelay = 80;
	Serial_println_s("Scanning for devices...please wait");
	Serial_println_s(NULL);
	for (uint8_t s = 0; s <= 0x7F; s++) {
		returnStatus = sendAddress(SLA_W(s), MODE_WRITE);
		if (returnStatus) {
			if (returnStatus == 1) {
				Serial_println_s
				    ("There is a problem with the bus, could not complete scan");
				timeOutDelay = tempTime;
				return;
			} else {
				Serial_print_s("scanning address 0x");
				Serial_print_ub(s, HEX);
				Serial_print_s("\r");
			}
		} else {
			Serial_print_s("Found device at address - ");
			Serial_print_s(" 0x");
			Serial_println_ub(s, HEX);
			totalDevicesFound++;
		}
		stop();
	}
	if (!totalDevicesFound) {
		Serial_println_s("No devices found");
	}
	timeOutDelay = tempTime;
}

uint8_t I2C_available()
{
	return (bytesAvailable);
}

uint8_t I2C_receive()
{
	bufferIndex = totalBytes - bytesAvailable;
	if (!bytesAvailable) {
		bufferIndex = 0;
		return (0);
	}
	bytesAvailable--;
	return (data[bufferIndex]);
}

/* --- write functions --------------------------------------------------- */

uint8_t I2C_write(uint8_t address, uint8_t registerAddress)
{
	return (I2C_write_sn(address, registerAddress, NULL, 0));
}

uint8_t I2C_write_reg(uint8_t address, uint8_t registerAddress, uint8_t data)
{
	return (I2C_write_sn(address, registerAddress, &data, 1));
}

uint8_t I2C_write_s(uint8_t address, uint8_t registerAddress, char *data)
{
	return (I2C_write_sn(address, registerAddress, data, strlen(data)));
}

uint8_t I2C_write_sn(uint8_t address, uint8_t registerAddress, uint8_t * data,
		     uint8_t numberBytes)
{
	SEND_ADDRESS_W(address);
	SEND_BYTE(registerAddress);
// only this part differs for the variants of I2C_write()
	while (numberBytes--) {
		SEND_BYTE(*data++);
	}
//
	returnStatus = stop();
	if (returnStatus == 1) {
		return (7);
	}
	return (returnStatus);
}

/** read multiple bytes from I2C bus into internal data buffer
 *
 * @param address: I2C address to be read
 * @param numberBytes: Number of bytes to be read. 0 is treated as one.
 * @returns: error code. Possible values:
 *    0: ok
 *    5: Waiting for ACK/NACK while addressing slave in receiver mode (MR)
 *    6: Waiting for ACK/NACK while receiving data from the slave
 */
uint8_t I2C_read(uint8_t address, uint8_t numberBytes)
{
	return (I2C_readbuf(address, numberBytes, data));
}

/**
 * write register address and read one byte
 *
 * This implements a single-byte write transfer followed by a multi-byte read.
 */
uint8_t I2C_read_reg(uint8_t address, uint8_t registerAddress,
		     uint8_t numberBytes)
{
	SEND_ADDRESS_W(address);
	SEND_BYTE(registerAddress);
// now the regular read part with a repeated start
	return (I2C_readbuf(address, numberBytes, data));
}

/** read multiple bytes from I2C bus into external buffer
 *
 * @param address: I2C address to be read
 * @param numberBytes: Number of bytes to be read. 0 is treated as one.
 * @param dataBuffer: where to store the received bytes
 * @returns: error code. Possible values:
 *    0: ok
 *    5: Waiting for ACK/NACK while addressing slave in receiver mode (MR)
 *    6: Waiting for ACK/NACK while receiving data from the slave
 */
uint8_t I2C_readbuf(uint8_t address, uint8_t numberBytes, uint8_t * dataBuffer)
{
	uint16_t startingTime;

	totalBytes = 0;
	bytesAvailable = 0;
	bufferIndex = 0;
	if (numberBytes == 0) {
		numberBytes = 1;
	}
	// method 2 (see RM0016, page 293):

	returnStatus = sendAddress(SLA_R(address), numberBytes);
	if (returnStatus) {
		if (returnStatus == 1) {
			return (5);
		}
		return (returnStatus);
	}

	startingTime = millis();
	if (numberBytes == 1) {
		// method 2, case single byte (see RM0016, page 294):
//		I2C->CR2 |= I2C_CR2_STOP;	// send stop after receiving the one data byte
		returnStatus = receiveByte();	// wait for RxNE flag
		if (returnStatus == 1)
			return (6);
		if (returnStatus) {
			return (returnStatus);
		}
		dataBuffer[0] = I2C->DR;	// save the data
		bytesAvailable = 1;
		totalBytes = 1;
	} else if (numberBytes == 2) {
		// method 2, case two bytes (see RM0016, page 294):
		// Case of two bytes to be received:
//		I2C->CR2 &= ~I2C_CR2_ACK;	// clear ACK
		TIMEOUT_WAIT_FOR_ONE(I2C->SR1 & I2C_SR1_BTF, 6);	// Wait for BTF to be set
		// masking interrupts according to errata sheet #17140 rev. 5
		BEGIN_CRITICAL
			I2C->CR2 |= I2C_CR2_STOP;	// Program STOP
			dataBuffer[0] = I2C->DR;	// Read DR twice
		END_CRITICAL
		dataBuffer[1] = I2C->DR;
		bytesAvailable = 2;
		totalBytes = 2;
	} else {
		uint8_t tmp1, tmp2;
		// method 2, general case, n>2 (see RM0016, page 294):
		while (numberBytes > 3) {
			TIMEOUT_WAIT_FOR_ONE(I2C->SR1 & I2C_SR1_BTF, 6);	// Wait for BTF to be set
			dataBuffer[bytesAvailable++] = I2C->DR;	// save the data
			totalBytes++;
			numberBytes--;	//FIXME: while ans Schleifenende
		}
		TIMEOUT_WAIT_FOR_ONE(I2C->SR1 & I2C_SR1_BTF, 6);	// Wait for BTF to be set
		I2C->CR2 &= ~I2C_CR2_ACK;		// clear ACK
		// masking interrupts according to errata sheet #17140 rev. 5
		// using temporary variables to keep the critical section as
		// short as possible. The pointer arithmetics for dataBuffer
		// compiles to quite complex code.
		//TODO: Could be optimized with assembler code
		BEGIN_CRITICAL
			tmp1 = I2C->DR;			// read DataN-2
			I2C->CR2 |= I2C_CR2_STOP;	// Program STOP
			tmp2 = I2C->DR;			// read DataN-1
		END_CRITICAL
		dataBuffer[bytesAvailable++] = tmp1;
		totalBytes++;
		dataBuffer[bytesAvailable++] = tmp2;
		totalBytes++;
		returnStatus = receiveByte();	// wait for RxNE flag
		if (returnStatus == 1)
			return (6);
		if (returnStatus) {
			return (returnStatus);
		}
		dataBuffer[bytesAvailable++] = I2C->DR;	// read DataN
		totalBytes++;
	}
	TIMEOUT_WAIT_FOR_ZERO(I2C->CR2 & I2C_CR2_STOP, 7);	// Wait for STOP end
	return (returnStatus);
}

/**
 * write register address and read (multiple) bytes into memory
 *
 * This implements a single-byte write transfer followed by a multi-byte read.
 */
uint8_t I2C_readbuf_reg(uint8_t address, uint8_t registerAddress,
			uint8_t numberBytes, uint8_t * dataBuffer)
{
	SEND_ADDRESS_W(address);
	SEND_BYTE(registerAddress);
	// now a regular read (with a repeated start)
	return (I2C_readbuf(address, numberBytes, dataBuffer));
}

/////////////// Private Methods ////////////////////////////////////////

/**
 * send start condition and the target address and wait for the ADDR event
 *
 * The flag handling for POS and ACK is determined by the mode byte.
 * At the end, ADDR is cleared by reading CR3.
 *
 * @parms mode: set the flag handling for POS and ACK
 *		1: clear ACK in ADDR event, before clearing ADDR (receive 1)
 *		2: set ACK, POS before ADDR event (receive 2)
 *		3: set ACK before ADDR event (receive > 2, write)
 */
static uint8_t sendAddress(uint8_t i2cAddress, uint8_t mode)
{
	uint16_t startingTime = millis();

	/* do not wait for BUSY==0 as this would block for repeated start */
//	TIMEOUT_WAIT_FOR_ZERO((I2C->SR3 & I2C_SR3_BUSY), 1);

	I2C->CR2 |= I2C_CR2_ACK;	// set ACK
	/* send start sequence */
	I2C->CR2 |= I2C_CR2_START;	// send start sequence

	/* Test on EV5 and clear it (SB flag) */
	TIMEOUT_WAIT_FOR_ONE(I2C->SR1 & I2C_SR1_SB, 1);

	/* Send the Address + Direction */
	I2C->DR = i2cAddress;	// I2C_Send7bitAddress()

	/* Test on EV6, but don't clear it yet (ADDR flag) */
	// error code 2: no ACK received on address transmission
	TIMEOUT_WAIT_FOR_ONE(I2C->SR1 & I2C_SR1_ADDR, 2);

	if (mode == 1) {
		I2C->CR2 &= ~I2C_CR2_ACK;	// clear ACK
		BEGIN_CRITICAL			// disable interrupts
		(void) I2C->SR3;		// read SR3 to clear ADDR event bit
		I2C->CR2 |= I2C_CR2_STOP;	// send STOP soon
		END_CRITICAL			// enable interrupts
	} else if (mode == 2) {
		I2C->CR2 |= I2C_CR2_POS;	// set POS
		BEGIN_CRITICAL			// disable interrupts
		(void) I2C->SR3;		// read SR3 to clear ADDR event bit
		I2C->CR2 &= ~I2C_CR2_ACK;	// clear ACK
		END_CRITICAL			// enable interrupts
	} else {
		(void)I2C->SR3;		// read SR3 to clear ADDR event bit
	}

	return 0;
}

uint8_t sendByte(uint8_t i2cData)
{
	uint16_t startingTime = millis();

	/* Test on EV8 (wait for TXE flag) */
	/* On fail: 3: no ACK received on data transmission */
	TIMEOUT_WAIT_FOR_ONE((I2C->SR1 & I2C_SR1_TXE), 3);

	I2C->DR = i2cData;
	return 0;
}

/**
 * EV7: wait for RxNE flag and check for lost arbitration
 *
 * The actual data byte is not read but available in I2C->DR
 * @returns: error code. possible values:
 *     0: ok
 *     1: timeout while waiting for RxNE
 *     LOST_ARBTRTN: arbitration lost
 */
static uint8_t receiveByte(void)
{
	uint16_t startingTime = millis();
	/* Test on EV7 (BUSY, MSL and RXNE flags) */
	TIMEOUT_WAIT_FOR_ONE(I2C_CheckEvent(I2C_EVENT_MASTER_BYTE_RECEIVED), 1)
	    if (I2C->SR2 & I2C_SR2_ARLO)	// arbitration lost
	{
		lockUp();
		return (LOST_ARBTRTN);
	}
	return (0);
}

static uint8_t stop(void)
{
	uint16_t startingTime = millis();

	/* Test on EV8_2: TXE and BTF flags */
	TIMEOUT_WAIT_FOR_ONE((I2C->SR1 & (I2C_SR1_TXE | I2C_SR1_BTF)) ==
			     (I2C_SR1_TXE | I2C_SR1_BTF), 3);

	/* Generate a STOP condition */
	I2C->CR2 |= I2C_CR2_STOP;

	// wait for the end of the STOP condition
	//
	// The reference manual rm0016 is not clear on how to check for this
	// condition. Maybe BUSY, BTF, TRA or even MSL.
	// Waiting for BTF works.
	// AN3281, Fig. 4 specifies to wait for STOPF, but that does not work.
	// The source code attached to AN3281 waits for the STOP bit in CR2
	// to flip back to zero. This works, so this method is used.
//	TIMEOUT_WAIT_FOR_ONE((I2C->SR1 & I2C_SR1_BTF), 7);	// works
//	TIMEOUT_WAIT_FOR_ONE((I2C->SR1 & I2C_SR1_STOPF), 7);	// doesn't work
	TIMEOUT_WAIT_FOR_ZERO(I2C->CR2 & I2C_CR2_STOP, 7);	// works
	return (0);
}

static void lockUp(void)
{
#if 0
	TWCR = 0;		//releases SDA and SCL lines to high impedance
	TWCR = _BV(TWEN) | _BV(TWEA);	//reinitialize TWI 
#endif
	//FIXME: this needs to be checked in detail. CR1 might be involved
	// don't do a full software reset here. That would require a full
	// re-initialization before the next transfer could happen.
	I2C->CR2 = 0;
}
