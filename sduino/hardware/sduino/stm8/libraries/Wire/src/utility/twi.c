/*
  twi.c - TWI/I2C library for Wiring & Arduino
  Copyright (c) 2006 Nicholas Zambetti.  All right reserved.

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

  Modified 2012 by Todd Krein (todd@krein.org) to implement repeated starts
*/

/*
#include <math.h>
#include <stdlib.h>
#include <inttypes.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <compat/twi.h>
*/
#include "Arduino.h" // for digitalWrite

#ifndef cbi
#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
#endif

#ifndef sbi
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))
#endif

//#include "pins_arduino.h"
#include "twi.h"

static volatile uint8_t twi_state;
static volatile uint8_t twi_slarw;
static volatile uint8_t twi_sendStop;			// should the transaction end with a stop
static volatile uint8_t twi_inRepStart;			// in the middle of a repeated start

static void (*twi_onSlaveTransmit)(void);
static void (*twi_onSlaveReceive)(uint8_t*, int);

static uint8_t twi_masterBuffer[TWI_BUFFER_LENGTH];
static volatile uint8_t twi_masterBufferIndex;
static volatile uint8_t twi_masterBufferLength;

static uint8_t twi_txBuffer[TWI_BUFFER_LENGTH];
static volatile uint8_t twi_txBufferIndex;
static volatile uint8_t twi_txBufferLength;

static uint8_t twi_rxBuffer[TWI_BUFFER_LENGTH];
static volatile uint8_t twi_rxBufferIndex;

static volatile uint8_t twi_error;


/* --- SDuino additions -------------------------------------------------- */
/* This part is common with the I2C library
 */
static uint16_t twi_timeOutDelay;
static uint8_t returnStatus;

static uint8_t twi_sendAddress(uint8_t, uint8_t);
static uint8_t twi_sendByte(uint8_t);
static uint8_t twi_receiveByte(void);

static void twi_lockUp(void);

static uint16_t startingTime;
static bool timeout_expired;
static void tout_start(void);
static bool tout(void);

#define SLA_W(address)	(address << 1)
#define SLA_R(address)	((address << 1) + 0x01)

#define MODE_WRITE 4

#define SEND_ADDRESS_W(ADDR) \
	returnStatus = twi_sendAddress(SLA_W(ADDR),MODE_WRITE); \
	if (returnStatus) return(returnStatus);

// wait while the condition is still true (wait for a bit to become zero)
#define TIMEOUT_WAIT_FOR_ZERO(CONDITION,ERROR) \
	while (CONDITION) 	/* wait while the condition is still true */ \
	{ \
		if (tout()) \
		{ \
			twi_lockUp(); \
			return(ERROR);/* return the appropriate error code */ \
		} \
	}

// wait while the condition is not true (wait for a bit to become one)
#define TIMEOUT_WAIT_FOR_ONE(CONDITION,ERROR) TIMEOUT_WAIT_FOR_ZERO(!(CONDITION), ERROR)


/* --- public methods ---------------------------------------------------- */

/*
 * Function twi_init
 * Desc     readys twi pins and sets twi bitrate
 * Input    none
 * Output   none
 */
void twi_init(void)
{
	// set I2C frequency to 100kHz and do a full init
	twi_setFrequency(I2C_MAX_STANDARD_FREQ);

	// set default timeout to 20ms
	twi_timeOutDelay = 20;
}


/*
 * Function twi_disable
 * Desc     disables twi pins
 * Input    none
 * Output   none
 */
void twi_disable(void)
{
	I2C->CR1 = 0;
/*
  // disable twi module, acks, and twi interrupt
  TWCR &= ~(_BV(TWEN) | _BV(TWIE) | _BV(TWEA));

  // deactivate internal pullups for twi.
  digitalWrite(SDA, 0);
  digitalWrite(SCL, 0);
*/
}

/*
 * Function twi_slaveInit
 * Desc     sets slave address (FIXME: and enables interrupt)
 * Input    address to be set (gets shifted one bit left to skip the r/w bit)
 * Output   none
 */
void twi_setAddress(uint8_t address)
{
	// set twi slave address
	I2C->OARL = address << 1;
	I2C->OARH = I2C_OARH_ADDCONF;
}

/*
 * Function twi_setClock
 * Desc     sets twi bit rate
 * Input    Clock Frequency
 * Output   none
 */
void twi_setFrequency(uint32_t frequency)
{
	// the easiest way to change the frequency is a full re-init
	I2C_Init(
		frequency,		// I2C_SPEED,
		0xA0,			// OwnAddress, doesn't matter
		I2C_DUTYCYCLE_2,	// 0x00
		I2C_ACK_CURR,		// 0x01
		I2C_ADDMODE_7BIT,	// 0x00
		F_CPU/1000000u		// InputClockFrequencyMhz
	);
}

/*
 * Function twi_readFrom
 * Desc     attempts to become twi bus master and read a
 *          series of bytes from a device on the bus
 * Input    address: 7bit i2c device address
 *          data: pointer to byte array
 *          length: number of bytes to read into array
 *          sendStop: Boolean indicating whether to send a stop at the end
 * Output   number of bytes read
 *FIXME: no interrupt support, polling mode only
 */
uint8_t twi_readFrom(uint8_t address, uint8_t* data, uint8_t length, uint8_t sendStop)
{
	uint8_t bytesAvailable;

	(void) sendStop;		//FIXME: ignore sendStop for now
	bytesAvailable = 0;
	// method 2 (see RM0016, page 293):

	if (twi_sendAddress(SLA_R(address), length)) return 0;

	if (!length) return 0;
	tout_start();
	if (length == 1) {
		// method 2, case single byte (see RM0016, page 294):
//		I2C->CR2 |= I2C_CR2_STOP;	// send stop after receiving the one data byte
		// wait for RxNE flag
		if (twi_receiveByte()) return 0;
		// save the data
		data[0] = I2C->DR;
		bytesAvailable = 1;
	} else if (length == 2) {
		// method 2, case two bytes (see RM0016, page 294):
		// Case of two bytes to be received:
//		I2C->CR2 &= ~I2C_CR2_ACK;	// clear ACK
		// Wait for BTF to be set
		TIMEOUT_WAIT_FOR_ONE(I2C->SR1 & I2C_SR1_BTF, 0);//6);
		// masking interrupts according to errata sheet #17140 rev. 5
		BEGIN_CRITICAL
			I2C->CR2 |= I2C_CR2_STOP;	// Program STOP
			data[0] = I2C->DR;		// Read DR twice
		END_CRITICAL
		data[1] = I2C->DR;
		bytesAvailable = 2;
	} else {
		uint8_t tmp1, tmp2;
		// method 2, general case, n>2 (see RM0016, page 294):
		while (length > 3) {
			// Wait for BTF to be set
			TIMEOUT_WAIT_FOR_ONE(I2C->SR1 & I2C_SR1_BTF, bytesAvailable);//6);
			// save the data
			*data++ = I2C->DR;
			bytesAvailable++;
			length--;	//FIXME: while ans Schleifenende
		}
		// Wait for BTF to be set
		TIMEOUT_WAIT_FOR_ONE(I2C->SR1 & I2C_SR1_BTF, bytesAvailable);//6);
		// clear ACK
		I2C->CR2 &= ~I2C_CR2_ACK;
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
		*data++ = tmp1;
		*data++ = tmp2;
		bytesAvailable += 2;
		if (twi_receiveByte()) return bytesAvailable;// wait for RxNE flag
		*data = I2C->DR;	// read DataN
		bytesAvailable++;
	}
	// don't use the TIMEOUT macro. In the error case we would have to
	// return(bytesAvailable), which is done at the end anyway.
	// Wait for STOP end
	while (I2C->CR2 & I2C_CR2_STOP)
	{
		if (tout()) twi_lockUp();
	}
	return (bytesAvailable);
}
/* original Arduino code (for reference):
{
  uint8_t i;

  // ensure data will fit into buffer
  if(TWI_BUFFER_LENGTH < length){
    return 0;
  }

  // wait until twi is ready, become master receiver
  while(TWI_READY != twi_state){
    continue;
  }
  twi_state = TWI_MRX;
  twi_sendStop = sendStop;
  // reset error state (0xFF.. no error occured)
  twi_error = 0xFF;

  // initialize buffer iteration vars
  twi_masterBufferIndex = 0;
  twi_masterBufferLength = length-1;  // This is not intuitive, read on...
  // On receive, the previously configured ACK/NACK setting is transmitted in
  // response to the received byte before the interrupt is signalled.
  // Therefor we must actually set NACK when the _next_ to last byte is
  // received, causing that NACK to be sent in response to receiving the last
  // expected byte of data.

  // build sla+w, slave device address + w bit
  twi_slarw = TW_READ;
  twi_slarw |= address << 1;

  if (true == twi_inRepStart) {
    // if we're in the repeated start state, then we've already sent the start,
    // (@@@ we hope), and the TWI statemachine is just waiting for the address byte.
    // We need to remove ourselves from the repeated start state before we enable interrupts,
    // since the ISR is ASYNC, and we could get confused if we hit the ISR before cleaning
    // up. Also, don't enable the START interrupt. There may be one pending from the
    // repeated start that we sent ourselves, and that would really confuse things.
    twi_inRepStart = false;			// remember, we're dealing with an ASYNC ISR
    do {
      TWDR = twi_slarw;
    } while(TWCR & _BV(TWWC));
    TWCR = _BV(TWINT) | _BV(TWEA) | _BV(TWEN) | _BV(TWIE);	// enable INTs, but not START
  }
  else
    // send start condition
    TWCR = _BV(TWEN) | _BV(TWIE) | _BV(TWEA) | _BV(TWINT) | _BV(TWSTA);

  // wait for read operation to complete
  while(TWI_MRX == twi_state){
    continue;
  }

  if (twi_masterBufferIndex < length)
    length = twi_masterBufferIndex;

  // copy twi buffer to data
  for(i = 0; i < length; ++i){
    data[i] = twi_masterBuffer[i];
  }
  return length;
}
*/

/*
 * Function twi_writeTo
 * Desc     attempts to become twi bus master and write a
 *          series of bytes to a device on the bus
 * Input    address: 7bit i2c device address
 *          data: pointer to byte array
 *          length: number of bytes in array
 *          wait: boolean indicating to wait for write or not
 *          sendStop: boolean indicating whether or not to send a stop at the end
 * Output   0 .. success
 *          1 .. length to long for buffer
 *          2 .. address send, NACK received
 *          3 .. data send, NACK received
 *          4 .. other twi error (lost bus arbitration, bus error, ..)
 */
uint8_t twi_writeTo(uint8_t address, uint8_t* data, uint8_t length, uint8_t wait, uint8_t sendStop)
{
	(void) wait;	//FIXME: no interrupt support, ignore the wait parameter for now

	SEND_ADDRESS_W(address);
	while (length--) {
		if (twi_sendByte(*data++)) return (3);
	}

	if (sendStop) {
		twi_stop();
	}

	return 0;
}
/*
  // original Arduino code (for reference):
{
  uint8_t i;

  // ensure data will fit into buffer
  if(TWI_BUFFER_LENGTH < length){
    return 1;
  }

  // wait until twi is ready, become master transmitter
  while(TWI_READY != twi_state){
    continue;
  }
  twi_state = TWI_MTX;
  twi_sendStop = sendStop;
  // reset error state (0xFF.. no error occured)
  twi_error = 0xFF;

  // initialize buffer iteration vars
  twi_masterBufferIndex = 0;
  twi_masterBufferLength = length;

  // copy data to twi buffer
  for(i = 0; i < length; ++i){
    twi_masterBuffer[i] = data[i];
  }

  // build sla+w, slave device address + w bit
  twi_slarw = TW_WRITE;
  twi_slarw |= address << 1;

  // if we're in a repeated start, then we've already sent the START
  // in the ISR. Don't do it again.
  //
  if (true == twi_inRepStart) {
    // if we're in the repeated start state, then we've already sent the start,
    // (@@@ we hope), and the TWI statemachine is just waiting for the address byte.
    // We need to remove ourselves from the repeated start state before we enable interrupts,
    // since the ISR is ASYNC, and we could get confused if we hit the ISR before cleaning
    // up. Also, don't enable the START interrupt. There may be one pending from the
    // repeated start that we sent outselves, and that would really confuse things.
    twi_inRepStart = false;			// remember, we're dealing with an ASYNC ISR
    do {
      TWDR = twi_slarw;
    } while(TWCR & _BV(TWWC));
    TWCR = _BV(TWINT) | _BV(TWEA) | _BV(TWEN) | _BV(TWIE);	// enable INTs, but not START
  }
  else
    // send start condition
    TWCR = _BV(TWINT) | _BV(TWEA) | _BV(TWEN) | _BV(TWIE) | _BV(TWSTA);	// enable INTs

  // wait for write operation to complete
  while(wait && (TWI_MTX == twi_state)){
    continue;
  }

  if (twi_error == 0xFF)
    return 0;	// success
  else if (twi_error == TW_MT_SLA_NACK)
    return 2;	// error: address send, nack received
  else if (twi_error == TW_MT_DATA_NACK)
    return 3;	// error: data send, nack received
  else
    return 4;	// other twi error
  return 0;
}
*/

/*
 * Function twi_transmit
 * Desc     fills slave tx buffer with data
 *          must be called in slave tx event callback
 * Input    data: pointer to byte array
 *          length: number of bytes in array
 * Output   1 length too long for buffer
 *          2 not slave transmitter
 *          0 ok
 */
uint8_t twi_transmit(const uint8_t* data, uint8_t length)
{
	(void) data;
	(void) length;
/*
  uint8_t i;

  // ensure data will fit into buffer
  if(TWI_BUFFER_LENGTH < (twi_txBufferLength+length)){
    return 1;
  }

  // ensure we are currently a slave transmitter
  if(TWI_STX != twi_state){
    return 2;
  }

  // set length and copy data into tx buffer
  for(i = 0; i < length; ++i){
    twi_txBuffer[twi_txBufferLength+i] = data[i];
  }
  twi_txBufferLength += length;
*/

  return 0;
}

/*
 * Function twi_attachSlaveRxEvent
 * Desc     sets function called before a slave read operation
 * Input    function: callback function to use
 * Output   none
 */
/*
void twi_attachSlaveRxEvent( void (*function)(uint8_t*, int) )
{
  twi_onSlaveReceive = function;
}
*/

/*
 * Function twi_attachSlaveTxEvent
 * Desc     sets function called before a slave write operation
 * Input    function: callback function to use
 * Output   none
 */
/*
void twi_attachSlaveTxEvent( void (*function)(void) )
{
  twi_onSlaveTransmit = function;
}
*/

/*
 * Function twi_reply
 * Desc     sends byte or readys receive line
 * Input    ack: byte indicating to ack or to nack
 * Output   none
 */
/*
void twi_reply(uint8_t ack)
{
  // transmit master read ready signal, with or without ack
  if(ack){
    TWCR = _BV(TWEN) | _BV(TWIE) | _BV(TWINT) | _BV(TWEA);
  }else{
	  TWCR = _BV(TWEN) | _BV(TWIE) | _BV(TWINT);
  }
}
*/

/*
 * Function twi_stop
 * Desc     relinquishes bus master status
 * Input    none
 * Output   none
 */
void twi_stop(void)
{
	tout_start();

	/* Test on EV8_2: TXE and BTF flags */
//	TIMEOUT_WAIT_FOR_ONE((I2C->SR1 & (I2C_SR1_TXE | I2C_SR1_BTF)) ==
//			     (I2C_SR1_TXE | I2C_SR1_BTF));
	while ((I2C->SR1 & (I2C_SR1_TXE | I2C_SR1_BTF)) != (I2C_SR1_TXE | I2C_SR1_BTF))
	{
		if (tout())
        	{
	        	twi_lockUp();
	        	return; 	// don't update twi_state
		}
	}

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
//	TIMEOUT_WAIT_FOR_ZERO(I2C->CR2 & I2C_CR2_STOP);	// works
	while (I2C->CR2 & I2C_CR2_STOP)		// works
	{
		if (tout())
        	{
	        	twi_lockUp();
	        	return; 	// don't update twi_state
		}
	}

/*
  // send stop condition
  TWCR = _BV(TWEN) | _BV(TWIE) | _BV(TWEA) | _BV(TWINT) | _BV(TWSTO);

  // wait for stop condition to be exectued on bus
  // TWINT is not set after a stop condition!
  while(TWCR & _BV(TWSTO)){
    continue;
  }
*/

  // update twi state
  twi_state = TWI_READY;
}

/*
 * Function twi_releaseBus
 * Desc     releases bus control
 * Input    none
 * Output   none
 */
/*
void twi_releaseBus(void)
{
  // release bus
  TWCR = _BV(TWEN) | _BV(TWIE) | _BV(TWEA) | _BV(TWINT);

  // update twi state
  twi_state = TWI_READY;
}
*/

/*
ISR(TWI_vect)
{
  switch(TW_STATUS){
    // All Master
    case TW_START:     // sent start condition
    case TW_REP_START: // sent repeated start condition
      // copy device address and r/w bit to output register and ack
      TWDR = twi_slarw;
      twi_reply(1);
      break;

    // Master Transmitter
    case TW_MT_SLA_ACK:  // slave receiver acked address
    case TW_MT_DATA_ACK: // slave receiver acked data
      // if there is data to send, send it, otherwise stop
      if(twi_masterBufferIndex < twi_masterBufferLength){
        // copy data to output register and ack
        TWDR = twi_masterBuffer[twi_masterBufferIndex++];
        twi_reply(1);
      }else{
	if (twi_sendStop)
          twi_stop();
	else {
	  twi_inRepStart = true;	// we're gonna send the START
	  // don't enable the interrupt. We'll generate the start, but we
	  // avoid handling the interrupt until we're in the next transaction,
	  // at the point where we would normally issue the start.
	  TWCR = _BV(TWINT) | _BV(TWSTA)| _BV(TWEN) ;
	  twi_state = TWI_READY;
	}
      }
      break;
    case TW_MT_SLA_NACK:  // address sent, nack received
      twi_error = TW_MT_SLA_NACK;
      twi_stop();
      break;
    case TW_MT_DATA_NACK: // data sent, nack received
      twi_error = TW_MT_DATA_NACK;
      twi_stop();
      break;
    case TW_MT_ARB_LOST: // lost bus arbitration
      twi_error = TW_MT_ARB_LOST;
      twi_releaseBus();
      break;

    // Master Receiver
    case TW_MR_DATA_ACK: // data received, ack sent
      // put byte into buffer
      twi_masterBuffer[twi_masterBufferIndex++] = TWDR;
    case TW_MR_SLA_ACK:  // address sent, ack received
      // ack if more bytes are expected, otherwise nack
      if(twi_masterBufferIndex < twi_masterBufferLength){
        twi_reply(1);
      }else{
        twi_reply(0);
      }
      break;
    case TW_MR_DATA_NACK: // data received, nack sent
      // put final byte into buffer
      twi_masterBuffer[twi_masterBufferIndex++] = TWDR;
	if (twi_sendStop)
          twi_stop();
	else {
	  twi_inRepStart = true;	// we're gonna send the START
	  // don't enable the interrupt. We'll generate the start, but we
	  // avoid handling the interrupt until we're in the next transaction,
	  // at the point where we would normally issue the start.
	  TWCR = _BV(TWINT) | _BV(TWSTA)| _BV(TWEN) ;
	  twi_state = TWI_READY;
	}
	break;
    case TW_MR_SLA_NACK: // address sent, nack received
      twi_stop();
      break;
    // TW_MR_ARB_LOST handled by TW_MT_ARB_LOST case

    // Slave Receiver
    case TW_SR_SLA_ACK:   // addressed, returned ack
    case TW_SR_GCALL_ACK: // addressed generally, returned ack
    case TW_SR_ARB_LOST_SLA_ACK:   // lost arbitration, returned ack
    case TW_SR_ARB_LOST_GCALL_ACK: // lost arbitration, returned ack
      // enter slave receiver mode
      twi_state = TWI_SRX;
      // indicate that rx buffer can be overwritten and ack
      twi_rxBufferIndex = 0;
      twi_reply(1);
      break;
    case TW_SR_DATA_ACK:       // data received, returned ack
    case TW_SR_GCALL_DATA_ACK: // data received generally, returned ack
      // if there is still room in the rx buffer
      if(twi_rxBufferIndex < TWI_BUFFER_LENGTH){
        // put byte in buffer and ack
        twi_rxBuffer[twi_rxBufferIndex++] = TWDR;
        twi_reply(1);
      }else{
        // otherwise nack
        twi_reply(0);
      }
      break;
    case TW_SR_STOP: // stop or repeated start condition received
      // ack future responses and leave slave receiver state
      twi_releaseBus();
      // put a null char after data if there's room
      if(twi_rxBufferIndex < TWI_BUFFER_LENGTH){
        twi_rxBuffer[twi_rxBufferIndex] = '\0';
      }
      // callback to user defined callback
      twi_onSlaveReceive(twi_rxBuffer, twi_rxBufferIndex);
      // since we submit rx buffer to "wire" library, we can reset it
      twi_rxBufferIndex = 0;
      break;
    case TW_SR_DATA_NACK:       // data received, returned nack
    case TW_SR_GCALL_DATA_NACK: // data received generally, returned nack
      // nack back at master
      twi_reply(0);
      break;

    // Slave Transmitter
    case TW_ST_SLA_ACK:          // addressed, returned ack
    case TW_ST_ARB_LOST_SLA_ACK: // arbitration lost, returned ack
      // enter slave transmitter mode
      twi_state = TWI_STX;
      // ready the tx buffer index for iteration
      twi_txBufferIndex = 0;
      // set tx buffer length to be zero, to verify if user changes it
      twi_txBufferLength = 0;
      // request for txBuffer to be filled and length to be set
      // note: user must call twi_transmit(bytes, length) to do this
      twi_onSlaveTransmit();
      // if they didn't change buffer & length, initialize it
      if(0 == twi_txBufferLength){
        twi_txBufferLength = 1;
        twi_txBuffer[0] = 0x00;
      }
      // transmit first byte from buffer, fall
    case TW_ST_DATA_ACK: // byte sent, ack returned
      // copy data to output register
      TWDR = twi_txBuffer[twi_txBufferIndex++];
      // if there is more to send, ack, otherwise nack
      if(twi_txBufferIndex < twi_txBufferLength){
        twi_reply(1);
      }else{
        twi_reply(0);
      }
      break;
    case TW_ST_DATA_NACK: // received nack, we are done
    case TW_ST_LAST_DATA: // received ack, but we are done already!
      // ack future responses
      twi_reply(1);
      // leave slave receiver state
      twi_state = TWI_READY;
      break;

    // All
    case TW_NO_INFO:   // no state information
      break;
    case TW_BUS_ERROR: // bus error, illegal stop/start
      twi_error = TW_BUS_ERROR;
      twi_stop();
      break;
  }
}
*/

/* --- SDuino additions -------------------------------------------------- */

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
 * @returns: 0 .. success
 *          2 .. address send, NACK received
 *          4 .. other twi error (lost bus arbitration, bus error, ..)
 */
static uint8_t twi_sendAddress(uint8_t i2cAddress, uint8_t mode)
{
	tout_start();

	/* do not wait for BUSY==0 as this would block for repeated start */
//	TIMEOUT_WAIT_FOR_ZERO((I2C->SR3 & I2C_SR3_BUSY), 4);

	I2C->CR2 |= I2C_CR2_ACK;	// set ACK
	/* send start sequence */
	I2C->CR2 |= I2C_CR2_START;	// send start sequence

	/* Test on EV5 and clear it (SB flag) */
	TIMEOUT_WAIT_FOR_ONE(I2C->SR1 & I2C_SR1_SB, 4);

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

/**
 * send one data byte via I2C (blocking)
 *
 * @returns:0 .. success
 *          3 .. data send, NACK received
 */
uint8_t twi_sendByte(uint8_t i2cData)
{
	tout_start();

	/* Test on EV8 (wait for TXE flag) */
	/* On fail: 3: no ACK received on data transmission */
	TIMEOUT_WAIT_FOR_ONE(I2C->SR1 & I2C_SR1_TXE, 3);

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
static uint8_t twi_receiveByte(void)
{
	tout_start();
	/* Test on EV7 (BUSY, MSL and RXNE flags) */
	TIMEOUT_WAIT_FOR_ONE(I2C_CheckEvent(I2C_EVENT_MASTER_BYTE_RECEIVED), 1)
	    if (I2C->SR2 & I2C_SR2_ARLO)	// arbitration lost
	{
		twi_lockUp();
		return (2);//FIXME: should be LOST_ARBTRTN);
	}
	return (0);
}


static void twi_lockUp(void)
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

/**
 * set the timeout delay
 *
 * @params:
 *    ms: timeout value in ms. 0 to disable timeout (and wait indefinitly)
 */
void twi_setTimeout(uint16_t ms)
{
	twi_timeOutDelay = ms;
}


/**
 * start the timeout timer
 *
 * The current time is set as the starting time.
 */
static void tout_start(void)
{
	startingTime = millis();
	timeout_expired = false;
}

/**
 * check if the timeout period expired
 *
 * @returns:
 *   false: Still within the waiting period
 *   true: timeout expired
 */
static bool tout(void)
{
	if (!twi_timeOutDelay)
	{
		return false;	// no timeout set
	}
	if (!timeout_expired)
	{
		timeout_expired =
		((((uint16_t)millis()) - startingTime) >= twi_timeOutDelay);
	}
	return timeout_expired;
}
