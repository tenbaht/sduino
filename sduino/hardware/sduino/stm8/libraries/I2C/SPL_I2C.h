/**
 ******************************************************************************
 * @file    extracted from stm8s_i2c.h
 * @author  MCD Application Team
 * @version V2.2.0
 * @date    30-September-2014
 * @brief  This file contains all functions prototype and macros for the I2C
 *peripheral.
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; COPYRIGHT 2014 STMicroelectronics</center></h2>
 *
 * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
 * You may not use this file except in compliance with the License.
 * You may obtain a copy of the License at:
 *
 *        http://www.st.com/software_license_agreement_liberty_v2
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 ******************************************************************************
 */

#ifndef _ORIG_SPL_I2C_H
#define _ORIG_SPL_I2C_H

#if (ARDUINO >= 100)
#include <Arduino.h>
#else
#include <WProgram.h>
#endif

#ifndef __STM8S_I2C_H

/**
 * @brief I2C possible events
 * Values convention: 0xXXYY
 * XX = Event SR3 corresponding value
 * YY = Event SR1 corresponding value
 * @note if Event = EV3_2 the rule above does not apply
 * YY = Event SR2 corresponding value
 */

typedef enum {
  /*========================================

    I2C Master Events (Events grouped in order of communication)
  ==========================================*/
  /**
   * @brief  Communication start
   *
   * After sending the START condition (I2C_GenerateSTART() function) the master
   * has to wait for this event. It means that the Start condition has been
   * correctly released on the I2C bus (the bus is free, no other devices is
   * communicating).
   *
   */
  /* --EV5 */
  I2C_EVENT_MASTER_MODE_SELECT = (uint16_t)0x0301, /*!< BUSY, MSL and SB flag */

  /**
   * @brief  Address Acknowledge
   *
   * After checking on EV5 (start condition correctly released on the bus), the
   * master sends the address of the slave(s) with which it will communicate
   * (I2C_Send7bitAddress() function, it also determines the direction of the
   * communication: Master transmitter or Receiver). Then the master has to wait
   * that a slave acknowledges his address. If an acknowledge is sent on the
   * bus, one of the following events will be set:
   *
   *  1) In case of Master Receiver (7-bit addressing):
   *  the I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED event is set.
   *
   *  2) In case of Master Transmitter (7-bit addressing):
   *  the I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED is set
   *
   *  3) In case of 10-Bit addressing mode, the master (just after generating
   * the START and checking on EV5) has to send the header of 10-bit addressing
   * mode (I2C_SendData() function). Then master should wait on EV9. It means
   * that the 10-bit addressing header has been correctly sent on the bus. Then
   * master should send the second part of the 10-bit address (LSB) using the
   * function I2C_Send7bitAddress(). Then master should wait for event EV6.
   *
   */
  /* --EV6 */
  I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED =
      (uint16_t)0x0782, /*!< BUSY, MSL, ADDR, TXE and TRA flags */
  I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED =
      (uint16_t)0x0302, /*!< BUSY, MSL and ADDR flags */
  /* --EV9 */
  I2C_EVENT_MASTER_MODE_ADDRESS10 =
      (uint16_t)0x0308, /*!< BUSY, MSL and ADD10 flags */

  /**
   * @brief Communication events
   *
   * If a communication is established (START condition generated and slave
   * address acknowledged) then the master has to check on one of the following
   * events for communication procedures:
   *
   * 1) Master Receiver mode: The master has to wait on the event EV7 then to
   * read the data received from the slave (I2C_ReceiveData() function).
   *
   * 2) Master Transmitter mode: The master has to send data (I2C_SendData()
   *    function) then to wait on event EV8 or EV8_2.
   *    These two events are similar:
   *     - EV8 means that the data has been written in the data register and is
   *       being shifted out.
   *     - EV8_2 means that the data has been physically shifted out and output
   *       on the bus.
   *     In most cases, using EV8 is sufficient for the application.
   *     Using EV8_2 leads to a slower communication but ensures more reliable
   * test. EV8_2 is also more suitable than EV8 for testing on the last data
   * transmission (before Stop condition generation).
   *
   *  @note In case the user software does not guarantee that this event EV7 is
   *  managed before the current byte end of transfer, then user may check on
   * EV7 and BTF flag at the same time (ie. (I2C_EVENT_MASTER_BYTE_RECEIVED |
   * I2C_FLAG_BTF)). In this case the communication may be slower.
   *
   */
  /* Master RECEIVER mode -----------------------------*/
  /* --EV7 */
  I2C_EVENT_MASTER_BYTE_RECEIVED =
      (uint16_t)0x0340, /*!< BUSY, MSL and RXNE flags */

  /* Master TRANSMITTER mode --------------------------*/
  /* --EV8 */
  I2C_EVENT_MASTER_BYTE_TRANSMITTING =
      (uint16_t)0x0780, /*!< TRA, BUSY, MSL, TXE flags */
  /* --EV8_2 */

  I2C_EVENT_MASTER_BYTE_TRANSMITTED =
      (uint16_t)0x0784, /*!< EV8_2: TRA, BUSY, MSL, TXE and BTF flags */

  /*========================================

                       I2C Slave Events (Events grouped in order of
     communication)
                                                          ==========================================*/

  /**
   * @brief  Communication start events
   *
   * Wait on one of these events at the start of the communication. It means
   * that the I2C peripheral detected a Start condition on the bus (generated by
   * master device) followed by the peripheral address. The peripheral generates
   * an ACK condition on the bus (if the acknowledge feature is enabled through
   * function I2C_AcknowledgeConfig()) and the events listed above are set :
   *
   * 1) In normal case (only one address managed by the slave), when the address
   *   sent by the master matches the own address of the peripheral (configured
   * by I2C_OwnAddress1 field) the I2C_EVENT_SLAVE_XXX_ADDRESS_MATCHED event is
   * set (where XXX could be TRANSMITTER or RECEIVER).
   *
   * 2) In case the address sent by the master is General Call (address 0x00)
   * and if the General Call is enabled for the peripheral (using function
   * I2C_GeneralCallCmd()) the following event is set
   * I2C_EVENT_SLAVE_GENERALCALLADDRESS_MATCHED.
   *
   */

  /* --EV1  (all the events below are variants of EV1) */
  /* 1) Case of One Single Address managed by the slave */
  I2C_EVENT_SLAVE_RECEIVER_ADDRESS_MATCHED =
      (uint16_t)0x0202, /*!< BUSY and ADDR flags */
  I2C_EVENT_SLAVE_TRANSMITTER_ADDRESS_MATCHED =
      (uint16_t)0x0682, /*!< TRA, BUSY, TXE and ADDR flags */

  /* 2) Case of General Call enabled for the slave */
  I2C_EVENT_SLAVE_GENERALCALLADDRESS_MATCHED =
      (uint16_t)0x1200, /*!< EV2: GENCALL and BUSY flags */

  /**
   * @brief  Communication events
   *
   * Wait on one of these events when EV1 has already been checked :
   *
   * - Slave RECEIVER mode:
   *     - EV2: When the application is expecting a data byte to be received.
   *     - EV4: When the application is expecting the end of the communication:
   *       master sends a stop condition and data transmission is stopped.
   *
   * - Slave Transmitter mode:
   *    - EV3: When a byte has been transmitted by the slave and the application
   *      is expecting the end of the byte transmission.
   *      The two events I2C_EVENT_SLAVE_BYTE_TRANSMITTED and
   * I2C_EVENT_SLAVE_BYTE_TRANSMITTING are similar. The second one can
   * optionally be used when the user software doesn't guarantee the EV3 is
   * managed before the current byte end of transfer.
   *    - EV3_2: When the master sends a NACK in order to tell slave that data
   * transmission shall end (before sending the STOP condition). In this case
   * slave has to stop sending data bytes and expect a Stop condition on
   * the bus.
   *
   *  @note In case the  user software does not guarantee that the event EV2 is
   *  managed before the current byte end of transfer, then user may check on
   * EV2 and BTF flag at the same time (ie. (I2C_EVENT_SLAVE_BYTE_RECEIVED |
   * I2C_FLAG_BTF)). In this case the communication may be slower.
   *
   */
  /* Slave RECEIVER mode --------------------------*/
  /* --EV2 */
  I2C_EVENT_SLAVE_BYTE_RECEIVED = (uint16_t)0x0240, /*!< BUSY and RXNE flags */
  /* --EV4  */
  I2C_EVENT_SLAVE_STOP_DETECTED = (uint16_t)0x0010, /*!< STOPF flag */

  /* Slave TRANSMITTER mode -----------------------*/
  /* --EV3 */
  I2C_EVENT_SLAVE_BYTE_TRANSMITTED =
      (uint16_t)0x0684, /*!< TRA, BUSY, TXE and BTF flags */
  I2C_EVENT_SLAVE_BYTE_TRANSMITTING =
      (uint16_t)0x0680, /*!< TRA, BUSY and TXE flags */
  /* --EV3_2 */
  I2C_EVENT_SLAVE_ACK_FAILURE = (uint16_t)0x0004 /*!< AF flag */
} I2C_Event_TypeDef;

#endif
void Orig_I2C_Init(uint32_t OutputClockFrequencyHz, uint16_t OwnAddress,
                   uint8_t I2C_DutyCycle, uint8_t AddMode,
                   uint8_t InputClockFrequencyMHz);

uint8_t Orig_I2C_CheckEvent(I2C_Event_TypeDef I2C_Event);
uint8_t Orig_I2C_GetFlagStatus(uint16_t I2C_Flag);
#endif
