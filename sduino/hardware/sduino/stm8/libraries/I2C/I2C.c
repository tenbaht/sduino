
#if (ARDUINO >= 100)
#include <Arduino.h>
#else
#include <WProgram.h>
#endif

#ifndef USE_SPL
#include "SPL_I2C.h"
#define I2C_CheckEvent Orig_I2C_CheckEvent
#define I2C_GetFlagStatus Orig_I2C_GetFlagStatus
#endif

#include "I2C.h"

#define TIMEOUT 0x0FFF

static uint8_t ret;
static uint16_t t;

#ifndef I2C_BUFFER_SIZE
#define I2C_BUFFER_SIZE 32
#endif
static uint8_t internalBuffer[I2C_BUFFER_SIZE];
static uint8_t internalWritePtr = 0;
static uint8_t internalReadPtr = 0;

static uint8_t sendAddress(uint8_t);
static uint8_t sendByte(uint8_t);
static uint8_t start(void);
static uint8_t stop(uint8_t);

void I2C_begin() {

#ifdef USE_SPL
  I2C_Init(I2C_MAX_STANDARD_FREQ, // 100000// I2C_SPEED,
           0xA0,                  // OwnAddress, egal
           I2C_DUTYCYCLE_2,       // 0x00
           I2C_ACK_CURR,          // 0x01
           I2C_ADDMODE_7BIT,      // 0x00
           16);                   // InputClockFreqencyMhz
#else
  Orig_I2C_Init(((uint32_t)100000), // I2C_SPEED,I2C_MAX_STANDARD_FREQ
                0xA0,               // OwnAddress, egal
                0x00,               // I2C_DUTYCYCLE_2
                0x00,               // 0I2C_ADDMODE_7BIT
                16                  // InputClockFreqencyMhz
  );
#endif
}

#define stopIfNotSuccess(ret, reason)                                          \
  if (ret) {                                                                   \
    return stop(reason);                                                       \
  }

static uint8_t start() {
  ret = 0;
  t = 0xFFFF;
  while (t-- && I2C_GetFlagStatus((uint16_t)0x0302)) // I2C_FLAG_BUSBUSY
    ;
  if (!t) {
    // Error
    ret = 1;
    return ret;
  }

#ifdef USE_SPL
  I2C_GenerateSTART(ENABLE);
#else
  I2C->CR2 |= I2C_CR2_START;
#endif

  return ret;
}

uint8_t stop(uint8_t res) {
#ifdef USE_SPL
  I2C_GenerateSTOP(ENABLE);
#else
  I2C->CR2 |= I2C_CR2_STOP;
#endif
  return res;
}

uint8_t sendAddress(uint8_t i2cAddress) {
  /* Test on EV5 and clear it */
  t = 0x0FFF;
  while (t-- && !I2C_CheckEvent(I2C_EVENT_MASTER_MODE_SELECT))
    ;
  if (!t) {
    return 1;
  }

#ifdef USE_SPL
  I2C_SendData(i2cAddress);
#else
  /* Send the Address + Direction */
  I2C->DR = i2cAddress; // I2C_Send7bitAddress()
#endif

  // #define SLA_W(address)  (address << 1)
  // #define SLA_R(address)  ((address << 1) + 0x01
  if (i2cAddress & 0x01) {
    // Master Receiver SLA_R
    t = 0x0FFF;
    while (t-- & !I2C_CheckEvent(I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED))
      ;
    if (!t) {
      return 2;
    }
  } else {
    // Master Transmitter SLA_W
    t = 0x0FFF;
    while (t-- && !I2C_CheckEvent(I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
      ;
    if (!t) {
      return 2;
    }
  }
  /* Test on EV6 and clear it */

  return 0;
}

uint8_t sendByte(uint8_t i2cData) {

#ifdef USE_SPL
  I2C_SendData(i2cData);
#else
  I2C->DR = i2cData;
#endif
  /* Test on EV8 */
  t = 0xFFFF;
  while (t-- && !I2C_CheckEvent(I2C_EVENT_MASTER_BYTE_TRANSMITTING))
    ;
  if (!t) {
    return 1;
  }

  /* Test on EV8_2 */
  t = 0xFFFF;
  while (t-- && !I2C_CheckEvent(I2C_EVENT_MASTER_BYTE_TRANSMITTED))
    ;
  // if (!t) {
  //   return 2; // fail here?
  // }

  return 0;
}

uint8_t I2C_write(uint8_t address, uint8_t registerAddress) {
  ret = start();
  stopIfNotSuccess(ret, 1);

  ret = sendAddress(SLA_W(address));
  stopIfNotSuccess(ret, 2);

  ret = sendByte(registerAddress);
  stopIfNotSuccess(ret, 3);

  return (stop(0));
}

uint8_t I2C_write_c(uint8_t address, uint8_t registerAddress, uint8_t data) {
  ret = start();
  stopIfNotSuccess(ret, 1);

  ret = sendAddress(SLA_W(address));
  stopIfNotSuccess(ret, 2);

  ret = sendByte(registerAddress);
  stopIfNotSuccess(ret, 3);

  ret = sendByte(data);
  stopIfNotSuccess(ret, 4);

  return (stop(0));
}

uint8_t I2C_write_sn(uint8_t address, uint8_t registerAddress, uint8_t *data,
                     uint8_t numberBytes) {
  ret = start();
  stopIfNotSuccess(ret, 1);

  ret = sendAddress(SLA_W(address));
  stopIfNotSuccess(ret, 2);

  ret = sendByte(registerAddress);
  stopIfNotSuccess(ret, 3);

  for (uint8_t i = 0; i < numberBytes; i++) {
    ret = sendByte(data[i]);
    stopIfNotSuccess(ret, 4);
  }

  return (stop(0));
}

uint8_t I2C_receive(void) {
  uint8_t result = internalBuffer[internalReadPtr];
  internalReadPtr = (internalReadPtr++) % I2C_BUFFER_SIZE;
  return result;
}

uint8_t I2C_read(uint8_t address, uint8_t registerAddress,
                 uint8_t numberBytes) {

  ret = I2C_write(address, registerAddress);
  stopIfNotSuccess(ret, ret);

// ACK on each byte
#ifdef USE_SPL
  I2C_AcknowledgeConfig(I2C_ACK_CURR);
#else
  I2C->CR2 |= I2C_CR2_ACK;
  I2C->CR2 &= (uint8_t)(~I2C_CR2_POS);
#endif

  ret = start();
  stopIfNotSuccess(ret, 4);

  ret = sendAddress(SLA_R(address));
  stopIfNotSuccess(ret, 5);

  for (uint8_t i = 0; i < numberBytes; i++) {
    t = 0xFFFF; // give slave some time
    while (t-- && !I2C_CheckEvent(I2C_EVENT_MASTER_BYTE_RECEIVED))
      ;
    if (!t) {
      return (stop(6));
    }

    if (i == numberBytes - 1) {
    // NACK after next byte
#ifdef USE_SPL
      I2C_AcknowledgeConfig(I2C_ACK_NONE);
#else
      I2C->CR2 &= (uint8_t)(~I2C_CR2_ACK);
#endif
      stop(0);
    }

#ifdef USE_SPL
    internalBuffer[internalWritePtr] = I2C_ReceiveData();
#else
    internalBuffer[internalWritePtr] = (uint8_t)I2C->DR;
#endif
    internalWritePtr = (internalWritePtr++) % I2C_BUFFER_SIZE;
  }

  return 0;
}

uint8_t I2C_read_buffer(uint8_t address, uint8_t *buffer, uint8_t numberBytes) {

// ACK on each byte
#ifdef USE_SPL
  I2C_AcknowledgeConfig(I2C_ACK_CURR);
#else
  I2C->CR2 |= I2C_CR2_ACK;
  I2C->CR2 &= (uint8_t)(~I2C_CR2_POS);
#endif

  ret = start();
  stopIfNotSuccess(ret, 1);

  ret = sendAddress(SLA_R(address));
  stopIfNotSuccess(ret, 2);

  for (uint8_t i = 0; i < numberBytes; i++) {
    t = 0xFFFF; // give slave some time
    while (t-- && !I2C_CheckEvent(I2C_EVENT_MASTER_BYTE_RECEIVED))
      ;
    if (!t) {
      return (stop(3));
    }

    if (i == numberBytes - 1) {
    // NACK after next byte
#ifdef USE_SPL
      I2C_AcknowledgeConfig(I2C_ACK_NONE);
#else
      I2C->CR2 &= (uint8_t)(~I2C_CR2_ACK);
#endif
    }

#ifdef USE_SPL
    buffer[i] = I2C_ReceiveData();
#else
    buffer[i] = (uint8_t)I2C->DR;
#endif
  }

  return (stop(0));
}
