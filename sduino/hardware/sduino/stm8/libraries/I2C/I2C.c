
#if (ARDUINO >= 100)
#include <Arduino.h>
#else
#include <WProgram.h>
#endif

#ifndef USE_SPL
// include original init function
#include "SPL_I2C.h"
#define I2C_CheckEvent Orig_I2C_CheckEvent
#endif

#include "I2C.h"

#define TIMEOUT 0x0FFF
uint8_t t;

uint8_t ret;

static uint8_t sendAddress(uint8_t);
static uint8_t sendByte(uint8_t);
static void start(void);
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

static void start() {
#ifdef USE_SPL
  I2C_GenerateSTART(ENABLE);
#else
  I2C->CR2 |= I2C_CR2_START;
#endif
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
  while (!I2C_CheckEvent(I2C_EVENT_MASTER_MODE_SELECT))
    ;

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
    while (!I2C_CheckEvent(I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED))
      ;
  } else {
    // Master Transmitter SLA_W
    while (!I2C_CheckEvent(I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
      ;
  }
  /* Test on EV6 and clear it */

  return 0;
}

uint8_t sendByte(uint8_t i2cData) {
  /* Test on EV8 */
  while (!I2C_CheckEvent(I2C_EVENT_MASTER_BYTE_TRANSMITTING))
    ;

#ifdef USE_SPL
  I2C_SendData(i2cData);
#else
  I2C->DR = i2cData;
#endif

  /* Test on EV8_2 */
  while (!I2C_CheckEvent(I2C_EVENT_MASTER_BYTE_TRANSMITTED))
    ;

  return 0;
}

uint8_t I2C_write(uint8_t address, uint8_t registerAddress) {
  start();
  ret = sendAddress(SLA_W(address));
  if (ret) {
    return (stop(1));
  }
  ret = sendByte(registerAddress);
  if (ret) {
    return (stop(2));
  }
  return (stop(0));
}

uint8_t I2C_write_c(uint8_t address, uint8_t registerAddress, uint8_t data) {
  start();
  ret = sendAddress(SLA_W(address));
  if (ret) {
    return (stop(1));
  }
  ret = sendByte(registerAddress);
  if (ret) {
    return (stop(2));
  }
  ret = sendByte(data);
  if (ret) {
    return (stop(3));
  }
  return (stop(0));
}

uint8_t I2C_write_sn(uint8_t address, uint8_t registerAddress, uint8_t *data,
                     uint8_t numberBytes) {
  start();
  ret = sendAddress(SLA_W(address));
  if (ret) {
    return (stop(1));
  }
  ret = sendByte(registerAddress);
  if (ret) {
    return (stop(2));
  }
  for (uint8_t i = 0; i < numberBytes; i++) {
    ret = sendByte(data[i]);
    if (ret) {
      return (stop(3));
    }
  }

  return (stop(0));
}

uint8_t I2C_read(uint8_t address, uint8_t *buffer, uint8_t numberBytes) {
  uint16_t t;

  // ACK on each read byte
#ifdef USE_SPL
  I2C_AcknowledgeConfig(I2C_ACK_CURR);
#else
  I2C->CR2 |= I2C_CR2_ACK;
  I2C->CR2 &= (uint8_t)(~I2C_CR2_POS);
#endif

  start();
  ret = sendAddress(SLA_R(address));
  if (ret) {
    return (stop(1));
  }

  for (uint8_t i = 0; i < numberBytes; i++) {
    t = 0x0FFF; // give slave some time
    while (t-- && !I2C_CheckEvent(I2C_EVENT_MASTER_BYTE_RECEIVED))
      ;
    if (!t) {
      return (stop(2));
    }

    if (i == numberBytes - 1) {
    // NACK after next byte
#ifdef USE_SPL
      I2C_AcknowledgeConfig(I2C_ACK_NONE);
#else
      I2C->CR2 &= (uint8_t)(~I2C_CR2_ACK);
#endif
    }

    buffer[i] = I2C_ReceiveData();
  }

  return (stop(0));
}
