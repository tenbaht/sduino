/**
  ******************************************************************************
  * @file    stm8s_it.h
  * @author  MCD Application Team
  * @version V2.2.0
  * @date    30-September-2014
  * @brief   This file contains the headers of the interrupt handlers
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __STM8S_IT_H
#define __STM8S_IT_H

/* Includes ------------------------------------------------------------------*/
#include "stm8s.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
#ifdef _COSMIC_
 void _stext(void); /* RESET startup routine */
 INTERRUPT void NonHandledInterrupt(void);
#endif /* _COSMIC_ */

#if !defined(_RAISONANCE_) && !defined(_SDCC_)  // SDCC patch: interrupt keyword required after function
 INTERRUPT void TRAP_IRQHandler(void); /* TRAP */
 INTERRUPT void TLI_IRQHandler(void); /* TLI */
 INTERRUPT void AWU_IRQHandler(void); /* AWU */
 INTERRUPT void CLK_IRQHandler(void); /* CLOCK */
 INTERRUPT void EXTI_PORTA_IRQHandler(void); /* EXTI PORTA */
 INTERRUPT void EXTI_PORTB_IRQHandler(void); /* EXTI PORTB */
 INTERRUPT void EXTI_PORTC_IRQHandler(void); /* EXTI PORTC */
 INTERRUPT void EXTI_PORTD_IRQHandler(void); /* EXTI PORTD */
 INTERRUPT void EXTI_PORTE_IRQHandler(void); /* EXTI PORTE */

#if defined(STM8S903) || defined(STM8AF622x)
 INTERRUPT void EXTI_PORTF_IRQHandler(void); /* EXTI PORTF */
#endif /* (STM8S903) || (STM8AF622x) */

#if defined (STM8S208) || defined (STM8AF52Ax)
 INTERRUPT void CAN_RX_IRQHandler(void); /* CAN RX */
 INTERRUPT void CAN_TX_IRQHandler(void); /* CAN TX/ER/SC */
#endif /* (STM8S208) || (STM8AF52Ax) */

 INTERRUPT void SPI_IRQHandler(void); /* SPI */
 INTERRUPT void TIM1_CAP_COM_IRQHandler(void); /* TIM1 CAP/COM */
 INTERRUPT void TIM1_UPD_OVF_TRG_BRK_IRQHandler(void); /* TIM1 UPD/OVF/TRG/BRK */

#if defined(STM8S903) || defined(STM8AF622x)
 INTERRUPT void TIM5_UPD_OVF_BRK_TRG_IRQHandler(void); /* TIM5 UPD/OVF/BRK/TRG */
 INTERRUPT void TIM5_CAP_COM_IRQHandler(void); /* TIM5 CAP/COM */
#else /* (STM8S208) || (STM8S207) || (STM8S105) || (STM8S103) || (STM8AF52Ax) || (STM8AF62Ax) || (STM8A626x) */
 INTERRUPT void TIM2_UPD_OVF_BRK_IRQHandler(void); /* TIM2 UPD/OVF/BRK */
 INTERRUPT void TIM2_CAP_COM_IRQHandler(void); /* TIM2 CAP/COM */
#endif /* (STM8S903) || (STM8AF622x) */

#if defined (STM8S208) || defined(STM8S207) || defined(STM8S007) || defined(STM8S105) || \
    defined(STM8S005) ||  defined (STM8AF52Ax) || defined (STM8AF62Ax) || defined (STM8AF626x)
 INTERRUPT void TIM3_UPD_OVF_BRK_IRQHandler(void); /* TIM3 UPD/OVF/BRK */
 INTERRUPT void TIM3_CAP_COM_IRQHandler(void); /* TIM3 CAP/COM */
#endif /* (STM8S208) || (STM8S207) || (STM8S105) || (STM8AF52Ax) || (STM8AF62Ax) || (STM8A626x) */

#if defined (STM8S208) || defined(STM8S207) || defined(STM8S007) || defined(STM8S103) || \
    defined(STM8S003) ||  defined (STM8AF52Ax) || defined (STM8AF62Ax) || defined (STM8S903)
 INTERRUPT void UART1_TX_IRQHandler(void); /* UART1 TX */
 INTERRUPT void UART1_RX_IRQHandler(void); /* UART1 RX */
#endif /* (STM8S208) || (STM8S207) || (STM8S903) || (STM8S103) || (STM8AF52Ax) || (STM8AF62Ax) */

#if defined (STM8AF622x)
 INTERRUPT void UART4_TX_IRQHandler(void); /* UART4 TX */
 INTERRUPT void UART4_RX_IRQHandler(void); /* UART4 RX */
#endif /* (STM8AF622x) */
 
 INTERRUPT void I2C_IRQHandler(void); /* I2C */

#if defined(STM8S105) || defined(STM8S005) ||  defined (STM8AF626x)
 INTERRUPT void UART2_RX_IRQHandler(void); /* UART2 RX */
 INTERRUPT void UART2_TX_IRQHandler(void); /* UART2 TX */
#endif /* (STM8S105) || (STM8AF626x) */

#if defined(STM8S207) || defined(STM8S007) || defined(STM8S208) || defined (STM8AF52Ax) || defined (STM8AF62Ax)
 INTERRUPT void UART3_RX_IRQHandler(void); /* UART3 RX */
 INTERRUPT void UART3_TX_IRQHandler(void); /* UART3 TX */
#endif /* (STM8S207) || (STM8S208) || (STM8AF62Ax) || (STM8AF52Ax) */

#if defined(STM8S207) || defined(STM8S007) || defined(STM8S208) || defined (STM8AF52Ax) || defined (STM8AF62Ax)
 INTERRUPT void ADC2_IRQHandler(void); /* ADC2 */
#else /* (STM8S105) || (STM8S103) || (STM8S903) || (STM8AF622x) */
 INTERRUPT void ADC1_IRQHandler(void); /* ADC1 */
#endif /* (STM8S207) || (STM8S208) || (STM8AF62Ax) || (STM8AF52Ax) */

#if defined(STM8S903) || defined(STM8AF622x)
 INTERRUPT void TIM6_UPD_OVF_TRG_IRQHandler(void); /* TIM6 UPD/OVF/TRG */
#else /* (STM8S208) || (STM8S207) || (STM8S105) || (STM8S103) || (STM8AF62Ax) || (STM8AF52Ax) || (STM8AF626x) */
 INTERRUPT void TIM4_UPD_OVF_IRQHandler(void); /* TIM4 UPD/OVF */
#endif /* (STM8S903) || (STM8AF622x) */
 INTERRUPT void EEPROM_EEC_IRQHandler(void); /* EEPROM ECC CORRECTION */
#endif /* _RAISONANCE_ */

#if defined(_SDCC_)  // SDCC patch: interrupt keyword required after function
//FIXME void TRAP_IRQHandler(void) __interrupt(); /* TRAP */
// void TLI_IRQHandler(void) __interrupt(ITC_IRQ_TLI); /* TLI */
// void AWU_IRQHandler(void) __interrupt(ITC_IRQ_AWU); /* AWU */
// void CLK_IRQHandler(void) __interrupt(ITC_IRQ_CLK); /* CLOCK */
 void EXTI_PORTA_IRQHandler(void) __interrupt(ITC_IRQ_PORTA); /* EXTI PORTA */
 void EXTI_PORTB_IRQHandler(void) __interrupt(ITC_IRQ_PORTB); /* EXTI PORTB */
 void EXTI_PORTC_IRQHandler(void) __interrupt(ITC_IRQ_PORTC); /* EXTI PORTC */
 void EXTI_PORTD_IRQHandler(void) __interrupt(ITC_IRQ_PORTD); /* EXTI PORTD */
 void EXTI_PORTE_IRQHandler(void) __interrupt(ITC_IRQ_PORTE); /* EXTI PORTE */

#if defined(STM8S903) || defined(STM8AF622x)
 void EXTI_PORTF_IRQHandler(void) __interrupt(ITC_IRQ_PORTF); /* EXTI PORTF */
#endif /* (STM8S903) || (STM8AF622x) */

#if defined (STM8S208) || defined (STM8AF52Ax)
// void CAN_RX_IRQHandler(void) __interrupt(ITC_IRQ_CAN_RX); /* CAN RX */
// void CAN_TX_IRQHandler(void) __interrupt(ITC_IRQ_CAN_TX); /* CAN TX/ER/SC */
#endif /* (STM8S208) || (STM8AF52Ax) */

// void SPI_IRQHandler(void) __interrupt(ITC_IRQ_SPI); /* SPI */
 void TIM1_CAP_COM_IRQHandler(void) __interrupt(ITC_IRQ_TIM1_CAPCOM); /* TIM1 CAP/COM */
// void TIM1_UPD_OVF_TRG_BRK_IRQHandler(void) __interrupt(ITC_IRQ_TIM1_OVF); /* TIM1 UPD/OVF/TRG/BRK */

#if defined(STM8S903) || defined(STM8AF622x)
// void TIM5_UPD_OVF_BRK_TRG_IRQHandler(void) __interrupt(ITC_IRQ_TIM5_OVFTRI); /* TIM5 UPD/OVF/BRK/TRG */
// void TIM5_CAP_COM_IRQHandler(void) __interrupt(ITC_IRQ_TIM5_CAPCOM); /* TIM5 CAP/COM */
#else /* (STM8S208) || (STM8S207) || (STM8S105) || (STM8S103) || (STM8AF52Ax) || (STM8AF62Ax) || (STM8A626x) */
// void TIM2_UPD_OVF_BRK_IRQHandler(void) __interrupt(ITC_IRQ_TIM2_OVF); /* TIM2 UPD/OVF/BRK */
// void TIM2_CAP_COM_IRQHandler(void) __interrupt(ITC_IRQ_TIM2_CAPCOM); /* TIM2 CAP/COM */
#endif /* (STM8S903) || (STM8AF622x) */

#if defined (STM8S208) || defined(STM8S207) || defined(STM8S007) || defined(STM8S105) || \
    defined(STM8S005) ||  defined (STM8AF52Ax) || defined (STM8AF62Ax) || defined (STM8AF626x)
// void TIM3_UPD_OVF_BRK_IRQHandler(void) __interrupt(ITC_IRQ_TIM3_OVF); /* TIM3 UPD/OVF/BRK */
// void TIM3_CAP_COM_IRQHandler(void) __interrupt(ITC_IRQ_TIM3_CAPCOM); /* TIM3 CAP/COM */
#endif /* (STM8S208) || (STM8S207) || (STM8S105) || (STM8AF52Ax) || (STM8AF62Ax) || (STM8A626x) */

#if defined (STM8S208) || defined(STM8S207) || defined(STM8S007) || defined(STM8S103) || \
    defined(STM8S003) ||  defined (STM8AF52Ax) || defined (STM8AF62Ax) || defined (STM8S903)
 void UART1_TX_IRQHandler(void) __interrupt(ITC_IRQ_UART1_TX); /* UART1 TX */
 void UART1_RX_IRQHandler(void) __interrupt(ITC_IRQ_UART1_RX); /* UART1 RX */
#endif /* (STM8S208) || (STM8S207) || (STM8S903) || (STM8S103) || (STM8AF52Ax) || (STM8AF62Ax) */

#if defined (STM8AF622x)
// void UART4_TX_IRQHandler(void) __interrupt(ITC_IRQ_UART4_TX); /* UART4 TX */
// void UART4_RX_IRQHandler(void) __interrupt(ITC_IRQ_UART4_RX); /* UART4 RX */
#endif /* (STM8AF622x) */
 
// void I2C_IRQHandler(void) __interrupt(ITC_IRQ_I2C); /* I2C */

#if defined(STM8S105) || defined(STM8S005) ||  defined (STM8AF626x)
 void UART2_RX_IRQHandler(void) __interrupt(ITC_IRQ_UART2_RX); /* UART2 RX */
 void UART2_TX_IRQHandler(void) __interrupt(ITC_IRQ_UART2_TX); /* UART2 TX */
#endif /* (STM8S105) || (STM8AF626x) */

#if defined(STM8S207) || defined(STM8S007) || defined(STM8S208) || defined (STM8AF52Ax) || defined (STM8AF62Ax)
// void UART3_RX_IRQHandler(void) __interrupt(ITC_IRQ_UART3_RX); /* UART3 RX */
// void UART3_TX_IRQHandler(void) __interrupt(ITC_IRQ_UART3_TX); /* UART3 TX */
#endif /* (STM8S207) || (STM8S208) || (STM8AF62Ax) || (STM8AF52Ax) */

#if defined(STM8S207) || defined(STM8S007) || defined(STM8S208) || defined (STM8AF52Ax) || defined (STM8AF62Ax)
// void ADC2_IRQHandler(void) __interrupt(ITC_IRQ_ADC2); /* ADC2 */
#else /* (STM8S105) || (STM8S103) || (STM8S903) || (STM8AF622x) */
// void ADC1_IRQHandler(void) __interrupt(ITC_IRQ_ADC1); /* ADC1 */
#endif /* (STM8S207) || (STM8S208) || (STM8AF62Ax) || (STM8AF52Ax) */

#if defined(STM8S903) || defined(STM8AF622x)
// void TIM6_UPD_OVF_TRG_IRQHandler(void) __interrupt(ITC_IRQ_TIM6_OVFTRI); /* TIM6 UPD/OVF/TRG */
#else /* (STM8S208) || (STM8S207) || (STM8S105) || (STM8S103) || (STM8AF62Ax) || (STM8AF52Ax) || (STM8AF626x) */
 void TIM4_UPD_OVF_IRQHandler(void) __interrupt(ITC_IRQ_TIM4_OVF); /* TIM4 UPD/OVF */
#endif /* (STM8S903) || (STM8AF622x) */
// void EEPROM_EEC_IRQHandler(void) __interrupt(ITC_IRQ_EEPROM_EEC); /* EEPROM ECC CORRECTION */
#endif /* _SDCC_ */

#endif /* __STM8S_IT_H */


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
