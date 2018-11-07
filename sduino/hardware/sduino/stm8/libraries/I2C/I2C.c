/*
  I2C.c - I2C library
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
#include "I2C.h"
#include "Serial.h"



uint8_t returnStatus;
//uint8_t nack;
uint8_t data[MAX_BUFFER_SIZE];

static uint8_t bytesAvailable = 0;
static uint8_t bufferIndex = 0;
static uint8_t totalBytes = 0;
static uint16_t timeOutDelay = 0;

/* --- internal building blocks ------------------------------------------ */
static uint8_t I2C_read_common(uint8_t address, uint8_t numberBytes);
static uint8_t start(void);
static uint8_t sendAddress(uint8_t, uint8_t);
static uint8_t sendByte(uint8_t);
static uint8_t receiveByte(void);
static uint8_t stop(void);
static void lockUp(void);

#define SEND_ADDRESS(ADDR) \
  returnStatus = start(); \
  if(returnStatus){return(returnStatus);} \
  returnStatus = sendAddress((ADDR),1); \
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

#define SEND_STOP \
  returnStatus = stop(); \
  if(returnStatus) \
  { \
    if(returnStatus == 1){return(7);} \
    return(returnStatus); \
  }


#define TIMEOUT_WAIT_WHILE(CONDITION,ERROR) \
	while (CONDITION) 	/* wait for the required condition */ \
	{ \
          if(!timeOutDelay){continue;} \
          if((((uint16_t)millis()) - startingTime) >= timeOutDelay) \
          { \
            lockUp(); \
            return(ERROR); 		/* return the appropriate error code */ \
          } \
        }


////////////// Public Methods ////////////////////////////////////////


void I2C_begin()
{
#if 0
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
  // initialize twi prescaler and bit rate
  cbi(TWSR, TWPS0);
  cbi(TWSR, TWPS1);
  TWBR = ((F_CPU / 100000) - 16) / 2;
  // enable twi module and acks
  TWCR = _BV(TWEN) | _BV(TWEA); 
#else 
	/* I2C Initialize */
	I2C_Init(
		I2C_MAX_STANDARD_FREQ,	// 100000// I2C_SPEED,
		0xA0,		// OwnAddress, egal
		I2C_DUTYCYCLE_2,	// 0x00
		I2C_ACK_CURR,		// 0x01
		I2C_ADDMODE_7BIT,	// 0x00
		16		// InputClockFreqencyMhz
	);
#endif
}

#if 0
void I2C_end()
{
  TWCR = 0;
}
#endif

void I2C_timeOut(uint16_t _timeOut)
{
  timeOutDelay = _timeOut;
}

#if 0
void I2C_setSpeed(uint8_t _fast)
{
  if(!_fast)
  {
    TWBR = ((F_CPU / 100000) - 16) / 2;
  }
  else
  {
    TWBR = ((F_CPU / 400000) - 16) / 2;
  }
}
#endif

#if 0
void I2C_pullup(uint8_t activate)
{
  if(activate)
  {
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
  }
  else
  {
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
  I2C_timeOut(80);
  uint8_t totalDevicesFound = 0;
  Serial_println_s("Scanning for devices...please wait");
  Serial_println_s(NULL);
  for(uint8_t s = 0; s <= 0x7F; s++)
  {
    returnStatus = 0;
    returnStatus = start();
    if(!returnStatus)
    { 
      returnStatus = sendAddress(SLA_W(s),1);
    }
    if(returnStatus)
    {
      if(returnStatus == 1)
      {
        Serial_println_s("There is a problem with the bus, could not complete scan");
        timeOutDelay = tempTime;
        return;
      }
      else
      {
        Serial_print_s("scanning address 0x");
        Serial_print_ub(s,HEX);
        Serial_print_s("\r");
      }
    }
    else
    {
      Serial_print_s("Found device at address - ");
      Serial_print_s(" 0x");
      Serial_println_ub(s,HEX);
      totalDevicesFound++;
    }
    stop();
  }
  if(!totalDevicesFound){Serial_println_s("No devices found");}
  timeOutDelay = tempTime;
}

uint8_t I2C_available()
{
  return(bytesAvailable);
}

uint8_t I2C_receive()
{
  bufferIndex = totalBytes - bytesAvailable;
  if(!bytesAvailable)
  {
    bufferIndex = 0;
    return(0);
  }
  bytesAvailable--;
  return(data[bufferIndex]);
}
  
/*return values for new functions that use the timeOut feature 
  will now return at what point in the transmission the timeout
  occurred. Looking at a full communication sequence between a 
  master and slave (transmit data and then readback data) there
  a total of 7 points in the sequence where a timeout can occur.
  These are listed below and correspond to the returned value:
  1 - Waiting for successful completion of a Start bit
  2 - Waiting for ACK/NACK while addressing slave in transmit mode (MT)
  3 - Waiting for ACK/NACK while sending data to the slave
  4 - Waiting for successful completion of a Repeated Start
  5 - Waiting for ACK/NACK while addressing slave in receiver mode (MR)
  6 - Waiting for ACK/NACK while receiving data from the slave
  7 - Waiting for successful completion of the Stop bit

  All possible return values:
  0           Function executed with no errors
  1 - 7       Timeout occurred, see above list
  8 - 0xFF    See datasheet for exact meaning */ 


/////////////////////////////////////////////////////

uint8_t I2C_write(uint8_t address, uint8_t registerAddress)
{
	return (I2C_write_sn(address, registerAddress, NULL, 0));
}


uint8_t I2C_write_c(uint8_t address, uint8_t registerAddress, uint8_t data)
{
	return (I2C_write_sn(address, registerAddress, &data, 1));
}


uint8_t I2C_write_s(uint8_t address, uint8_t registerAddress, char *data)
{
	return (I2C_write_sn(address, registerAddress, data, strlen(data)));
}


uint8_t I2C_write_sn(uint8_t address, uint8_t registerAddress, uint8_t *data, uint8_t numberBytes)
{
  SEND_ADDRESS(SLA_W(address));
  SEND_BYTE(registerAddress);
// only this part differs for the variants of I2C_write()
  while (numberBytes--)
  {
    SEND_BYTE(*data++);
  }
//
  SEND_STOP
  return(returnStatus);
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
uint8_t I2C_read_reg(uint8_t address, uint8_t registerAddress, uint8_t numberBytes)
{
  SEND_ADDRESS(SLA_W(address));
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
uint8_t I2C_readbuf(uint8_t address, uint8_t numberBytes, uint8_t *dataBuffer)
{
  bytesAvailable = 0;
  bufferIndex = 0;
  if(numberBytes == 0){numberBytes++;}

  returnStatus = start();
  if(returnStatus){return(returnStatus);}

  // method 2 (see RM0016, page 293):

  returnStatus = sendAddress(SLA_R(address),(numberBytes>1));
  if(returnStatus)
  {
    if(returnStatus == 1){return(5);}
    return(returnStatus);
  }

  if (numberBytes==1) {
    // method 2, case single byte (see RM0016, page 294):
    I2C->CR2 &= ~I2C_CR2_ACK;	// clear ACK
    (void) I2C->SR3;		// read SR3 to clear ADDR condition
    I2C->CR2 |= I2C_CR2_STOP;	// send stop after receiving the one data byte
    returnStatus = receiveByte();// wait for RxNE flag
    if (returnStatus == 1) return(6);
    if(returnStatus){return(returnStatus);}
    dataBuffer[0] = I2C->DR;	// save the data
    bytesAvailable = 1;
    totalBytes = 1;
  } else if (numberBytes==2) {
    //FIXME: case n=2
    // method 2, case two bytes (see RM0016, page 294):
  } else {
    // method 2, general case, n>2 (see RM0016, page 294):
    //FIXME: case n>2
  }
  return(returnStatus);
}


/**
 * write register address and read (multiple) bytes into memory
 *
 * This implements a single-byte write transfer followed by a multi-byte read.
 */
uint8_t I2C_readbuf_reg(uint8_t address, uint8_t registerAddress, uint8_t numberBytes, uint8_t *dataBuffer)
{
  SEND_ADDRESS(SLA_W(address));
  SEND_BYTE(registerAddress);
// now a regular read with a repeated start
  return (I2C_readbuf(address, numberBytes, dataBuffer));
}


/////////////// Private Methods ////////////////////////////////////////

/**
 * send a start sequence
 *
 * in contrast to the AVR version it does not wait for start to finish for
 * the STM8. The timeout is handled in sendAddress().
 */
static uint8_t start(void)
{
#if 0
  unsigned long startingTime = millis();
  TWCR = (1<<TWINT)|(1<<TWSTA)|(1<<TWEN);
  while (!(TWCR & (1<<TWINT)))
  {
    if(!timeOutDelay){continue;}
    if((millis() - startingTime) >= timeOutDelay)
    {
      lockUp();
      return(1);
    }
       
  }
  if ((TWI_STATUS == START) || (TWI_STATUS == REPEATED_START))
  {
    return(0);
  }
  if (TWI_STATUS == LOST_ARBTRTN)
  {
    uint8_t bufferedStatus = TWI_STATUS;
    lockUp();
    return(bufferedStatus);
  }
  return(TWI_STATUS);
#else
	I2C->CR2 |= I2C_CR2_START;	// send start sequence
	return 0;
#endif
}

static uint8_t sendAddress(uint8_t i2cAddress, uint8_t clear_addr)
{
#if 0
  TWDR = i2cAddress;
  unsigned long startingTime = millis();
  TWCR = (1<<TWINT) | (1<<TWEN);
  while (!(TWCR & (1<<TWINT)))
  {
    if(!timeOutDelay){continue;}
    if((millis() - startingTime) >= timeOutDelay)
    {
      lockUp();
      return(1);
    }
       
  }
  if ((TWI_STATUS == MT_SLA_ACK) || (TWI_STATUS == MR_SLA_ACK))
  {
    return(0);
  }
  uint8_t bufferedStatus = TWI_STATUS;
  if ((TWI_STATUS == MT_SLA_NACK) || (TWI_STATUS == MR_SLA_NACK))
  {
    stop();
    return(bufferedStatus);
  }
  else
  {
    lockUp();
    return(bufferedStatus);
  } 
#else
        uint16_t startingTime = millis();

	/* Test on EV5 and clear it (BUSY, MSL and SB flag) */
	while (!I2C_CheckEvent(I2C_EVENT_MASTER_MODE_SELECT));

	/* Send the Address + Direction */
	I2C->DR = i2cAddress;	// I2C_Send7bitAddress()

	/* Test on EV6, but don't clear it yet (ADDR flag) */
        // error code 2: no ACK received on address transmission
	TIMEOUT_WAIT_WHILE(!(I2C->SR1 & I2C_SR1_ADDR),2);

	if (clear_addr) {
		(void) I2C->SR3;	// read SR3 to clear ADDR event bit
	}

	return 0;
#endif
}

uint8_t sendByte(uint8_t i2cData)
{
#if 0
  TWDR = i2cData;
  unsigned long startingTime = millis();
  TWCR = (1<<TWINT) | (1<<TWEN);
  while (!(TWCR & (1<<TWINT)))
  {
    if(!timeOutDelay){continue;}
    if((millis() - startingTime) >= timeOutDelay)
    {
      lockUp();
      return(1);
    }
       
  }
  if (TWI_STATUS == MT_DATA_ACK)
  {
    return(0);
  }
  uint8_t bufferedStatus = TWI_STATUS;
  if (TWI_STATUS == MT_DATA_NACK)
  {
    stop();
    return(bufferedStatus);
  }
  else
  {
    lockUp();
    return(bufferedStatus);
  } 
#else
        uint16_t startingTime = millis();

	/* Test on EV8 (TRA, BUSY, MSL, TXE flags) */
	/* On fail: 3: no ACK received on data transmission */
	TIMEOUT_WAIT_WHILE(!I2C_CheckEvent(I2C_EVENT_MASTER_BYTE_TRANSMITTING), 3);

	I2C->DR = i2cData;
	return 0;
#endif
}

#if 0
static uint8_t receiveByte(uint8_t ack)
{
  unsigned long startingTime = millis();
  if(ack)
  {
    TWCR = (1<<TWINT) | (1<<TWEN) | (1<<TWEA);
  }
  else
  {
    TWCR = (1<<TWINT) | (1<<TWEN);
  }
  while (!(TWCR & (1<<TWINT)))
  {
    if(!timeOutDelay){continue;}
    if((millis() - startingTime) >= timeOutDelay)
    {
      lockUp();
      return(1);
    }
  }
  if (TWI_STATUS == LOST_ARBTRTN)
  {
    uint8_t bufferedStatus = TWI_STATUS;
    lockUp();
    return(bufferedStatus);
  }
  return(TWI_STATUS); 
#else
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
  TIMEOUT_WAIT_WHILE(!I2C_CheckEvent(I2C_EVENT_MASTER_BYTE_RECEIVED), 1)
  if (I2C->SR2 & I2C_SR2_ARLO)		// arbitration lost
  {
    lockUp();
    return(LOST_ARBTRTN);
  }
  return(0);
#endif
}

static uint8_t stop(void)
{
#if 0
  unsigned long startingTime = millis();
  TWCR = (1<<TWINT)|(1<<TWEN)| (1<<TWSTO);
  while ((TWCR & (1<<TWSTO)))
  {
    if(!timeOutDelay){continue;}
    if((millis() - startingTime) >= timeOutDelay)
    {
      lockUp();
      return(1);
    }
       
  }
#else
	uint16_t startingTime = millis();

	/* Test on EV8_2 */
	TIMEOUT_WAIT_WHILE(!I2C_CheckEvent(I2C_EVENT_MASTER_BYTE_TRANSMITTED), 3);
	// error 3: no ACK received on data transmission

	/* Generate a STOP condition */
	I2C->CR2 |= I2C_CR2_STOP;

	TIMEOUT_WAIT_WHILE(!(I2C->SR1 & I2C_SR1_STOPF), 7);
#endif
  return(0);
}

static void lockUp(void)
{
#if 0
  TWCR = 0; //releases SDA and SCL lines to high impedance
  TWCR = _BV(TWEN) | _BV(TWEA); //reinitialize TWI 
#endif
        //FIXME: this needs to be checked in detail. CR1 might be involved
	I2C->CR2 = 0;
}
