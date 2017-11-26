/**
 ******************************************************************************
 * @file    extracted from stm8s_i2c.c
 * @author  MCD Application Team
 * @version V2.2.0
 * @date    30-September-2014
 * @brief   This file contains all the functions for the I2C peripheral.
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

#include <SPL_I2C.h>

/**
 * @brief  Initializes the I2C according to the specified parameters in standard
 *         or fast mode.
 * @param  OutputClockFrequencyHz : Specifies the output clock frequency in Hz.
 * @param  OwnAddress : Specifies the own address.
 * @param  I2C_DutyCycle : Specifies the duty cycle to apply in fast mode.
 *         This parameter can be any of the  @ref I2C_DutyCycle_TypeDef
 * enumeration.
 * @note   This parameter don't have impact when the OutputClockFrequency lower
 *         than 100KHz.
 * @param  Ack : Specifies the acknowledge mode to apply.
 *         This parameter can be any of the  @ref I2C_Ack_TypeDef enumeration.
 * @param  AddMode : Specifies the acknowledge address to apply.
 *         This parameter can be any of the  @ref I2C_AddMode_TypeDef
 * enumeration.
 * @param  InputClockFrequencyMHz : Specifies the input clock frequency in MHz.
 * @retval None
 */
void Orig_I2C_Init(uint32_t OutputClockFrequencyHz, uint16_t OwnAddress,
                   uint8_t I2C_DutyCycle,
                   uint8_t AddMode,
                   uint8_t InputClockFrequencyMHz) {
  uint16_t result = 0x0004;
  uint16_t tmpval = 0;
  uint8_t tmpccrh = 0;

  /*------------------------- I2C FREQ Configuration ------------------------*/
  /* Clear frequency bits */
  I2C->FREQR &= (uint8_t)(~I2C_FREQR_FREQ);
  /* Write new value */
  I2C->FREQR |= InputClockFrequencyMHz;

  /*--------------------------- I2C CCR Configuration ------------------------*/
  /* Disable I2C to configure TRISER */
  I2C->CR1 &= (uint8_t)(~I2C_CR1_PE);

  /* Clear CCRH & CCRL */
  I2C->CCRH &= (uint8_t)(~(I2C_CCRH_FS | I2C_CCRH_DUTY | I2C_CCRH_CCR));
  I2C->CCRL &= (uint8_t)(~I2C_CCRL_CCR);

  /* Detect Fast or Standard mode depending on the Output clock frequency
   * selected */
  if (OutputClockFrequencyHz > I2C_MAX_STANDARD_FREQ) /* FAST MODE */
  {
    /* Set F/S bit for fast mode */
    tmpccrh = I2C_CCRH_FS;

    if (I2C_DutyCycle == I2C_DUTYCYCLE_2) {
      /* Fast mode speed calculate: Tlow/Thigh = 2 */
      result = (uint16_t)((InputClockFrequencyMHz * 1000000) /
                          (OutputClockFrequencyHz * 3));
    } else /* I2C_DUTYCYCLE_16_9 */
    {
      /* Fast mode speed calculate: Tlow/Thigh = 16/9 */
      result = (uint16_t)((InputClockFrequencyMHz * 1000000) /
                          (OutputClockFrequencyHz * 25));
      /* Set DUTY bit */
      tmpccrh |= I2C_CCRH_DUTY;
    }

    /* Verify and correct CCR value if below minimum value */
    if (result < (uint16_t)0x01) {
      /* Set the minimum allowed value */
      result = (uint16_t)0x0001;
    }

    /* Set Maximum Rise Time: 300ns max in Fast Mode
    = [300ns/(1/InputClockFrequencyMHz.10e6)]+1
    = [(InputClockFrequencyMHz * 3)/10]+1 */
    tmpval = ((InputClockFrequencyMHz * 3) / 10) + 1;
    I2C->TRISER = (uint8_t)tmpval;

  } else /* STANDARD MODE */
  {

    /* Calculate standard mode speed */
    result = (uint16_t)((InputClockFrequencyMHz * 1000000) /
                        (OutputClockFrequencyHz << (uint8_t)1));

    /* Verify and correct CCR value if below minimum value */
    if (result < (uint16_t)0x0004) {
      /* Set the minimum allowed value */
      result = (uint16_t)0x0004;
    }

    /* Set Maximum Rise Time: 1000ns max in Standard Mode
    = [1000ns/(1/InputClockFrequencyMHz.10e6)]+1
    = InputClockFrequencyMHz+1 */
    I2C->TRISER = (uint8_t)(InputClockFrequencyMHz + (uint8_t)1);
  }

  /* Write CCR with new calculated value */
  I2C->CCRL = (uint8_t)result;
  I2C->CCRH =
      (uint8_t)((uint8_t)((uint8_t)(result >> 8) & I2C_CCRH_CCR) | tmpccrh);

  /* Enable I2C */
  I2C->CR1 |= I2C_CR1_PE;

  /* Configure I2C acknowledgement */
  I2C->CR2 |= I2C_CR2_ACK;
  I2C->CR2 &= (uint8_t)(~I2C_CR2_POS);

  /*--------------------------- I2C OAR Configuration ------------------------*/
  I2C->OARL = (uint8_t)(OwnAddress);
  I2C->OARH =
      (uint8_t)((uint8_t)(AddMode | I2C_OARH_ADDCONF) |
                (uint8_t)((OwnAddress & (uint16_t)0x0300) >> (uint8_t)7));
}

uint8_t Orig_I2C_CheckEvent(I2C_Event_TypeDef I2C_Event) {
  __IO uint16_t lastevent = 0x00;
  uint8_t flag1 = 0x00;
  uint8_t flag2 = 0x00;
  uint8_t status = 0;

  /* Check the parameters */
  // assert_param(IS_I2C_EVENT_OK(I2C_Event));

  if (I2C_Event == I2C_EVENT_SLAVE_ACK_FAILURE) {
    lastevent = I2C->SR2 & I2C_SR2_AF;
  } else {
    flag1 = I2C->SR1;
    flag2 = I2C->SR3;
    lastevent = ((uint16_t)((uint16_t)flag2 << (uint16_t)8) | (uint16_t)flag1);
  }
  /* Check whether the last event is equal to I2C_EVENT */
  if (((uint16_t)lastevent & (uint16_t)I2C_Event) == (uint16_t)I2C_Event) {
    /* SUCCESS: last event is equal to I2C_EVENT */
    status = !0;
  } else {
    /* ERROR: last event is different from I2C_EVENT */
    status = 0;
  }

  /* Return status */
  return status;
}

uint8_t Orig_I2C_GetFlagStatus(uint16_t I2C_Flag)
{
  uint8_t tempreg = 0;
  uint8_t regindex = 0;
  uint8_t bitstatus = RESET;

  /* Read flag register index */
  regindex = (uint8_t)((uint16_t)I2C_Flag >> 8);
  /* Check SRx index */
  switch (regindex)
  {
      /* Returns whether the status register to check is SR1 */
    case 0x01:
      tempreg = (uint8_t)I2C->SR1;
      break;

      /* Returns whether the status register to check is SR2 */
    case 0x02:
      tempreg = (uint8_t)I2C->SR2;
      break;

      /* Returns whether the status register to check is SR3 */
    case 0x03:
      tempreg = (uint8_t)I2C->SR3;
      break;

    default:
      break;
  }

  /* Check the status of the specified I2C flag */
  if ((tempreg & (uint8_t)I2C_Flag ) != 0)
  {
    /* Flag is set */
    bitstatus = !0;
  }
  else
  {
    /* Flag is reset */
    bitstatus = 0;
  }
  /* Return the flag status */
  return bitstatus;
}