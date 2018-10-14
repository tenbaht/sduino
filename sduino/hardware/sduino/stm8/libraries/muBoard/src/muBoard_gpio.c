/**
  \file muBoard_beep.c
   
  \author G. Icking-Konert
  \date 2018-10-14
  \version 0.1
   
  \brief implementation of beeper routine
   
  implementation of routines for beeper control
*/

/*----------------------------------------------------------
    INCLUDE FILES
----------------------------------------------------------*/
#include "muBoard.h"
#include "muBoard_gpio.h"


/*----------------------------------------------------------
    FUNCTIONS
----------------------------------------------------------*/

/**
  \fn void gpioInit(void)
   
  \brief initialize all muBoard GPIO pins
  
  Configure all GPIOs on the board to their correct state. 
*/
void gpioInit() {


  //////////////////
  // init ports
  //////////////////
  
  //////////////////////////////////////////////////////////////////////////
  // parallel bus first to avoid glitches on DACs
  
  /////////
  // PORT G:
  //    0: CAN Tx
  //    1: CAN Rx
  //    2: parallel bus 2
  //    3: parallel bus 3
  //    4: parallel bus 4
  //    5: parallel bus 5
  //    6: parallel bus 6
  //    7: parallel bus 7
  /////////
  GPIOG->ODR = 0b00000001;   // init outputs to low, except for CAN Tx
  GPIOG->DDR = 0b11111101;   // input(=0) or output(=1)
  GPIOG->CR1 = 0b11111101;   // input: 0=float, 1=pull-up; output: 0=open-drain, 1=push-pull
  GPIOG->CR2 = 0b11111101;   // input: 0=no exint, 1=exint; output: 0=2MHz slope, 1=10MHz slope
  
              
  /////////
  // PORT I:
  //    0: parallel bus 8
  //    1: parallel bus 9
  //    2: parallel bus 10 / IDAC channel
  //    3: parallel bus 11
  //    4: parallel bus 12
  //    5: parallel bus 13
  //    6: parallel bus 14
  //    7: parallel bus 15
  /////////
  GPIOI->ODR = 0b00000000;   // init outputs to low
  GPIOI->DDR = 0b11111111;   // input(=0) or output(=1)
  GPIOI->CR1 = 0b11111111;   // input: 0=float, 1=pull-up; output: 0=open-drain, 1=push-pull
  GPIOI->CR2 = 0b11111111;   // input: 0=no exint, 1=exint; output: 0=2MHz slope, 1=10MHz slope


  /////////
  // single pins of parallel bus 
  /////////

  // PC4: parallel bus 0
  ClrBit(GPIOC->ODR, 4);     // init outputs to low, except for DAC update 
  SetBit(GPIOC->DDR, 4);     // input(=0) or output(=1)
  SetBit(GPIOC->CR1, 4);     // input: 0=float, 1=pull-up; output: 0=open-drain, 1=push-pull
  SetBit(GPIOC->CR2, 4);     // input: 0=no exint, 1=exint; output: 0=2MHz slope, 1=10MHz slope

  // PE0: parallel bus 1
  ClrBit(GPIOE->ODR, 0);     // init outputs to low, except for DAC update 
  SetBit(GPIOE->DDR, 0);     // input(=0) or output(=1)
  SetBit(GPIOE->CR1, 0);     // input: 0=float, 1=pull-up; output: 0=open-drain, 1=push-pull
  SetBit(GPIOE->CR2, 0);     // input: 0=no exint, 1=exint; output: 0=2MHz slope, 1=10MHz slope


  // HW version v2.2 (without CAN) uses full PG/PI as parallel bus
#if (HW_VERSION==22)
  GPIOG->ODR = 0b00000000;   // init outputs to low, except for CAN Tx
  GPIOG->DDR = 0b11111111;   // input(=0) or output(=1)
  GPIOG->CR1 = 0b11111111;   // input: 0=float, 1=pull-up; output: 0=open-drain, 1=push-pull
  GPIOG->CR2 = 0b11111111;   // input: 0=no exint, 1=exint; output: 0=2MHz slope, 1=10MHz slope

  // HW version v2.3 (with CAN) IO's already defined above
#elif (HW_VERSION==23)
  // dummy

  // neither HW v2.2 nor v2.3 --> error
#else
  #error illegal HW version in file 'muDuino.h'
#endif // HW_VERSION


  // done with parallel bus
  //////////////////////////////////////////////////////////////////////////



  /////////
  // PORT A:
  //    0: non-existent
  //    1: XTAL1
  //    2: XTAL2
  //    3: LS_2 control
  //    4: USART Rx (-> PC)
  //    5: USART Tx (-> PC)
  //    6: GPIO 4 / EXINT
  //    7: non-existent
  /////////
  GPIOA->ODR = 0b00100000;  // init outputs to low, expect Tx
  GPIOA->DDR = 0b00101000;  // input(=0) or output(=1)
  GPIOA->CR1 = 0b10111001;  // input: 0=float, 1=pull-up; output: 0=open-drain, 1=push-pull
  GPIOA->CR2 = 0b00101000;  // input: 0=no exint, 1=exint; output: 0=2MHz slope, 1=10MHz slope
  
  
  /////////
  // PORT B:
  //    0: GPIO_ADC 15 / EXINT
  //    1: GPIO_ADC 14  
  //    2: volt. divider ADC (9/1)
  //    3: volt. divider ADC (1/1)
  //    4: NTC ADC
  //    5: Poti ADC
  //    6: GPIO_ADC 13
  //    7: GPIO_ADC 12
  /////////
  GPIOB->ODR = 0b00000000;   // init outputs to low
  GPIOB->DDR = 0b00000000;   // input(=0) or output(=1)
  GPIOB->CR1 = 0b00000000;   // input: 0=float, 1=pull-up; output: 0=open-drain, 1=push-pull
  GPIOB->CR2 = 0b00000000;   // input: 0=no exint, 1=exint; output: 0=2MHz slope, 1=10MHz slope
  
  
  /////////
  // PORT C:
  //    0: LS_3 control  
  //    1: Timer/PWM 1 (TIM1_CC1)
  //    2: VDAC 1 update
  //    3: VDAC 2 update
  //    4: parallel bus 0 (v2.3) / n.c. (v2.2)
  //    5: SPI CLK
  //    6: SPI MOSI
  //    7: SPI MISO
  /////////
  GPIOC->ODR = 0b00001100;   // init outputs to low, except for DAC update 
  GPIOC->DDR = 0b01111101;   // input(=0) or output(=1)
  GPIOC->CR1 = 0b11111101;   // input: 0=float, 1=pull-up; output: 0=open-drain, 1=push-pull
  GPIOC->CR2 = 0b01111101;   // input: 0=no exint, 1=exint; output: 0=2MHz slope, 1=10MHz slope
  
                   
  /////////
  // PORT D:
  //    0: GPIO 3
  //    1: SWIM
  //    2: GPIO 2 / EXINT
  //    3: Timer/PWM 2 (TIM2_CC2)
  //    4: Beeper
  //    5: LIN UART Tx
  //    6: LIN UART Rx
  //    7: mode select (autorun)
  /////////
  GPIOD->ODR = 0b00100000;   // init outputs to low, except LIN UART Tx
  GPIOD->DDR = 0b00110000;   // input(=0) or output(=1)
  GPIOD->CR1 = 0b10110010;   // input: 0=float, 1=pull-up; output: 0=open-drain, 1=push-pull
  GPIOD->CR2 = 0b00110000;   // input: 0=no exint, 1=exint; output: 0=2MHz slope, 1=10MHz slope
  

  /////////
  // PORT E:
  //    0: parallel bus 1 (v2.3) / n.c. (v2.2)
  //    1: I2C SCL
  //    2: I2C SDA
  //    3: LCD reset
  //    4: IDAC update
  //    5: GPIO 1 / EXINT
  //    6: LS_1 control
  //    7: HV-bridge out 4
  /////////
  GPIOE->ODR = 0b00000110;   // init outputs to low, except for diff ampl update
  GPIOE->DDR = 0b11011111;   // input(=0) or output(=1)
  GPIOE->CR1 = 0b11011001;   // input: 0=float, 1=pull-up; output: 0=open-drain, 1=push-pull
  GPIOE->CR2 = 0b11011111;   // input: 0=no exint, 1=exint; output: 0=2MHz slope, 1=10MHz slope
  
  
  /////////
  // PORT F:
  //    0: GPIO_ADC 11 / CSN for SD card (initialized on first write to SD card)
  //    1: non-existent
  //    2: non-existent
  //    3: GPIO_ADC 10
  //    4: GPIO_ADC 9
  //    5: GPIO_ADC 8
  //    6: GPIO_ADC 7
  //    7: Vcap ADC (1.8V core voltage -> detect 3.3V / 5V supply)
  /////////
  GPIOF->ODR = 0b00000000;   // init outputs to low
  GPIOF->DDR = 0b00000000;   // input(=0) or output(=1)
  GPIOF->CR1 = 0b00000110;   // input: 0=float, 1=pull-up; output: 0=open-drain, 1=push-pull
  GPIOF->CR2 = 0b00000000;   // input: 0=no exint, 1=exint; output: 0=2MHz slope, 1=10MHz slope
  
  
  /////////
  // PORT H:
  //    0: GPIO 5
  //    1: GPIO 6
  //    2: status LED green
  //    3: status LED red
  //    4: GPIO 16 (Timer 1 trigger)
  //    5: HV-bridge out 1
  //    6: HV-bridge out 2
  //    7: HV-bridge out 3
  /////////
  GPIOH->ODR = 0b00001100;   // init outputs to low (exception status LEDs 1=off)
  GPIOH->DDR = 0b11101100;   // input(=0) or output(=1)
  GPIOH->CR1 = 0b11101100;   // input: 0=float, 1=pull-up; output: 0=open-drain, 1=push-pull
  GPIOH->CR2 = 0b11101100;   // input: 0=no exint, 1=exint; output: 0=2MHz slope, 1=10MHz slope
  
} // gpioInit



/**
  \fn void gpioMode(GPIO_TypeDef *pPort, uint8_t pin, uint8_t mode)
   
  \brief configure pin mode
  
  \param[in]  pPort  pointer to port, e.g. GPIOE
  \param[in]  pin    pin to configure (0..7)
  \param[in]  mode   new mode, e.g. INPUT_PULLUP (see muBoard_gpio.h)
  
  Configure pin as input or output mode with various options, e.g. pull-up or open-drain.
  See muBoard_gpio.h for supported modes. 
*/
void gpioMode(GPIO_TypeDef *pPort, uint8_t pin, uint8_t mode) {

   // set pin mode
   bitWrite(pPort->DDR, pin, mode & 0x04);     // input(=0) or output(=1)
   bitWrite(pPort->CR1, pin, mode & 0x02);     // input: 0=float, 1=pull-up; output: 0=open-drain, 1=push-pull
   bitWrite(pPort->CR2, pin, mode & 0x01);     // input: 0=no exint, 1=exint; output: 0=2MHz slope, 1=10MHz slope

} // gpioMode


/*-----------------------------------------------------------------------------
    END OF MODULE
-----------------------------------------------------------------------------*/
