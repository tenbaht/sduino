/* GPIO */
#define PA_ODR *(unsigned char*)0x5000
#define PA_IDR *(unsigned char*)0x5001
#define PA_DDR *(unsigned char*)0x5002
#define PA_CR1 *(unsigned char*)0x5003
#define PA_CR2 *(unsigned char*)0x5004

#define PB_ODR *(unsigned char*)0x5005
#define PB_IDR *(unsigned char*)0x5006
#define PB_DDR *(unsigned char*)0x5007
#define PB_CR1 *(unsigned char*)0x5008
#define PB_CR2 *(unsigned char*)0x5009

#define PC_ODR *(unsigned char*)0x500A
#define PC_IDR *(unsigned char*)0x500B
#define PC_DDR *(unsigned char*)0x500C
#define PC_CR1 *(unsigned char*)0x500D
#define PC_CR2 *(unsigned char*)0x500E

#define PD_ODR *(unsigned char*)0x500F
#define PD_IDR *(unsigned char*)0x5010
#define PD_DDR *(unsigned char*)0x5011
#define PD_CR1 *(unsigned char*)0x5012
#define PD_CR2 *(unsigned char*)0x5013

#define PE_ODR *(unsigned char*)0x5014
#define PE_IDR *(unsigned char*)0x5015
#define PE_DDR *(unsigned char*)0x5016
#define PE_CR1 *(unsigned char*)0x5017
#define PE_CR2 *(unsigned char*)0x5018

#define PF_ODR *(unsigned char*)0x5019
#define PF_IDR *(unsigned char*)0x501A
#define PF_DDR *(unsigned char*)0x501B
#define PF_CR1 *(unsigned char*)0x501C
#define PF_CR2 *(unsigned char*)0x501D

/* FLASH */
#define FLASH_CR1	*(unsigned char*)0x505A
#define FLASH_CR2	*(unsigned char*)0x505B
#define FLASH_NCR2	*(unsigned char*)0x505C
#define FLASH_FPR	*(unsigned char*)0x505D
#define FLASH_NFPR	*(unsigned char*)0x505E
#define FLASH_IAPSR	*(unsigned char*)0x505F
// unused				 0x5060
// unused				 0x5061
#define FLASH_PUKR	*(unsigned char*)0x5062
// unused				 0x5063
#define FLASH_DUKR	*(unsigned char*)0x5064

/* ITC */
#define EXTI_CR1	*(unsigned char*)0x50A0
#define EXTI_CR2	*(unsigned char*)0x50A1

/* RST */
#define RST_SR		*(unsigned char*)0x50B3

/* CLOCK */
#define CLK_ICKR	*(unsigned char*)0x50C0
#define CLK_ECKR	*(unsigned char*)0x50C1
// unused				 0x50C2
#define CLK_CMSR	*(unsigned char*)0x50C3
#define CLK_SWR 	*(unsigned char*)0x50C4
#define CLK_SWCR	*(unsigned char*)0x50C5
#define CLK_CKDIVR	*(unsigned char*)0x50C6
#define CLK_PCKENR1	*(unsigned char*)0x50C7
#define CLK_CSSR	*(unsigned char*)0x50C8
#define CLK_CCOR	*(unsigned char*)0x50C9
#define CLK_PCKENR2	*(unsigned char*)0x50CA
// unused				 0x50CB
#define CLK_HSITRIMR	*(unsigned char*)0x50CC
#define CLK_SWIMCCR	*(unsigned char*)0x50CD

/* WWDG */
#define WWDG_CR		*(unsigned char*)0x50D1
#define WWDG_WR		*(unsigned char*)0x50D2

/* IWDG */
#define IWDG_KR		*(unsigned char*)0x50E0
#define IWDG_PR		*(unsigned char*)0x50E1
#define IWDG_RLR	*(unsigned char*)0x50E2

/* AWU */
#define AWU_CSR1	*(unsigned char*)0x50F0
#define AWU_APR		*(unsigned char*)0x50F1
#define AWU_TBR		*(unsigned char*)0x50F2

/* BEEP */
#define BEEP_CSR	*(unsigned char*)0x50F3

/* SPI */
#define SPI_CR1		*(unsigned char*)0x5200
#define SPI_CR2		*(unsigned char*)0x5201
#define SPI_ICR		*(unsigned char*)0x5202
#define SPI_SR		*(unsigned char*)0x5203
#define SPI_DR		*(unsigned char*)0x5204
#define SPI_CRCPR	*(unsigned char*)0x5205
#define SPI_RXCRCR	*(unsigned char*)0x5206
#define SPI_TXCRCR	*(unsigned char*)0x5207

/* I2C */
#define I2C_CR1		*(unsigned char*)0x5210
#define I2C_CR2		*(unsigned char*)0x5211
#define I2C_FREQR	*(unsigned char*)0x5212
#define I2C_OARL	*(unsigned char*)0x5213
#define I2C_OARH	*(unsigned char*)0x5214
// unused				 0x5215
#define I2C_DR		*(unsigned char*)0x5216
#define I2C_SR1		*(unsigned char*)0x5217
#define I2C_SR2		*(unsigned char*)0x5218
#define I2C_SR3		*(unsigned char*)0x5219
#define I2C_ITR		*(unsigned char*)0x521A
#define I2C_CCRL	*(unsigned char*)0x521B
#define I2C_CCRH	*(unsigned char*)0x521C
#define I2C_TRISER	*(unsigned char*)0x521D
#define I2C_PECR	*(unsigned char*)0x521E

/* ------------------- USART ------------------- */
#define UART1_SR	*(unsigned char*)0x5230
#define UART1_DR	*(unsigned char*)0x5231
#define UART1_BRR1	*(unsigned char*)0x5232
#define UART1_BRR2	*(unsigned char*)0x5233
#define UART1_CR1	*(unsigned char*)0x5234
#define UART1_CR2	*(unsigned char*)0x5235
#define UART1_CR3	*(unsigned char*)0x5236
#define UART1_CR4	*(unsigned char*)0x5237
#define UART1_CR5	*(unsigned char*)0x5238
#define UART1_GTR	*(unsigned char*)0x5239
#define UART1_PSCR	*(unsigned char*)0x523A

/* USART_CR1 bits */
#define UART_CR1_R8	(1 << 7)
#define UART_CR1_T8	(1 << 6)
#define UART_CR1_UARTD	(1 << 5)
#define UART_CR1_M	(1 << 4)
#define UART_CR1_WAKE	(1 << 3)
#define UART_CR1_PCEN	(1 << 2)
#define UART_CR1_PS	(1 << 1)
#define UART_CR1_PIEN	(1 << 0)

/* USART_CR2 bits */
#define UART_CR2_TIEN (1 << 7)
#define UART_CR2_TCIEN (1 << 6)
#define UART_CR2_RIEN (1 << 5)
#define UART_CR2_ILIEN (1 << 4)
#define UART_CR2_TEN (1 << 3)
#define UART_CR2_REN (1 << 2)
#define UART_CR2_RWU (1 << 1)
#define UART_CR2_SBK (1 << 0)

/* USART_CR3 bits */
#define UART_CR3_LINEN (1 << 6)
#define UART_CR3_STOP2 (1 << 5)
#define UART_CR3_STOP1 (1 << 4)
#define UART_CR3_CLKEN (1 << 3)
#define UART_CR3_CPOL (1 << 2)
#define UART_CR3_CPHA (1 << 1)
#define UART_CR3_LBCL (1 << 0)

/* USART_SR bits */
#define UART_SR_TXE (1 << 7)
#define UART_SR_TC (1 << 6)
#define UART_SR_RXNE (1 << 5)
#define UART_SR_IDLE (1 << 4)
#define UART_SR_OR (1 << 3)
#define UART_SR_NF (1 << 2)
#define UART_SR_FE (1 << 1)
#define UART_SR_PE (1 << 0)
enum {	// UARTx_SR
    PE = 0,
    FE,
    NF,
    OR, LHE = 3,
    IDLE,
    RXNE,
    TC,
    TXE
};
    


/* ------------------- TIMERS ------------------- */

/* TIM1 */
#define TIM1_CR1	*(unsigned char*)0x5250
#define TIM1_CR2	*(unsigned char*)0x5251
#define TIM1_SMCR	*(unsigned char*)0x5252
#define TIM1_ETR	*(unsigned char*)0x5253
#define TIM1_IER	*(unsigned char*)0x5254
#define TIM1_SR1	*(unsigned char*)0x5255
#define TIM1_SR2	*(unsigned char*)0x5256
#define TIM1_EGR	*(unsigned char*)0x5257
#define TIM1_CCMR1	*(unsigned char*)0x5258
#define TIM1_CCMR2	*(unsigned char*)0x5259
#define TIM1_CCMR3	*(unsigned char*)0x525A
#define TIM1_CCMR4	*(unsigned char*)0x525B
#define TIM1_CCER1	*(unsigned char*)0x525C
#define TIM1_CCER2	*(unsigned char*)0x525D
#define TIM1_CNTRH	*(unsigned char*)0x525E
#define TIM1_CNTRL	*(unsigned char*)0x525F
#define TIM1_PSCRH	*(unsigned char*)0x5260
#define TIM1_PSCRL	*(unsigned char*)0x5261
#define TIM1_ARRH	*(unsigned char*)0x5262
#define TIM1_ARRL	*(unsigned char*)0x5263
#define TIM1_RCR	*(unsigned char*)0x5264
#define TIM1_CCR1H	*(unsigned char*)0x5265
#define TIM1_CCR1L	*(unsigned char*)0x5266
#define TIM1_CCR2H	*(unsigned char*)0x5267
#define TIM1_CCR2L	*(unsigned char*)0x5268
#define TIM1_CCR3H	*(unsigned char*)0x5269
#define TIM1_CCR3L	*(unsigned char*)0x526A
#define TIM1_CCR4H	*(unsigned char*)0x526B
#define TIM1_CCR4L	*(unsigned char*)0x526C
#define TIM1_BKR	*(unsigned char*)0x526D
#define TIM1_DTR	*(unsigned char*)0x526E
#define TIM1_OISR	*(unsigned char*)0x526F

/* TIM_IER bits */
#define TIM_IER_BIE (1 << 7)
#define TIM_IER_TIE (1 << 6)
#define TIM_IER_COMIE (1 << 5)
#define TIM_IER_CC4IE (1 << 4)
#define TIM_IER_CC3IE (1 << 3)
#define TIM_IER_CC2IE (1 << 2)
#define TIM_IER_CC1IE (1 << 1)
#define TIM_IER_UIE (1 << 0)

/* TIM_CR1 bits */
#define TIM_CR1_APRE (1 << 7)
#define TIM_CR1_CMSH (1 << 6)
#define TIM_CR1_CMSL (1 << 5)
#define TIM_CR1_DIR (1 << 4)
#define TIM_CR1_OPM (1 << 3)
#define TIM_CR1_URS (1 << 2)
#define TIM_CR1_UDIS (1 << 1)
#define TIM_CR1_CEN (1 << 0)

/* TIM_SR1 bits */
#define TIM_SR1_BIF (1 << 7)
#define TIM_SR1_TIF (1 << 6)
#define TIM_SR1_COMIF (1 << 5)
#define TIM_SR1_CC4IF (1 << 4)
#define TIM_SR1_CC3IF (1 << 3)
#define TIM_SR1_CC2IF (1 << 2)
#define TIM_SR1_CC1IF (1 << 1)
#define TIM_SR1_UIF (1 << 0)

/* TIM2 */
#define TIM2_CR1	*(unsigned char*)0x5300
// unused				 0x5301
// unused				 0x5302
#define TIM2_IER	*(unsigned char*)0x5303
#define TIM2_SR1	*(unsigned char*)0x5304
#define TIM2_SR2	*(unsigned char*)0x5305
#define TIM2_EGR	*(unsigned char*)0x5306
#define TIM2_CCMR1	*(unsigned char*)0x5307
#define TIM2_CCMR2	*(unsigned char*)0x5308
#define TIM2_CCMR3	*(unsigned char*)0x5309
#define TIM2_CCER1	*(unsigned char*)0x530A
#define TIM2_CCER2	*(unsigned char*)0x530B
#define TIM2_CNTRH	*(unsigned char*)0x530C
#define TIM2_CNTRL	*(unsigned char*)0x530D
#define TIM2_PSCR	*(unsigned char*)0x530E
#define TIM2_ARRH	*(unsigned char*)0x530F
#define TIM2_ARRL	*(unsigned char*)0x5310
#define TIM2_CCR1H	*(unsigned char*)0x5311
#define TIM2_CCR1L	*(unsigned char*)0x5312
#define TIM2_CCR2H	*(unsigned char*)0x5313
#define TIM2_CCR2L	*(unsigned char*)0x5314
#define TIM2_CCR3H	*(unsigned char*)0x5315
#define TIM2_CCR3L	*(unsigned char*)0x5316

/* TIM4 */
#define TIM4_CR1	*(unsigned char*)0x5340
// unused				 0x5341
// unused				 0x5342
#define TIM4_IER	*(unsigned char*)0x5343
#define TIM4_SR		*(unsigned char*)0x5344
#define TIM4_EGR	*(unsigned char*)0x5345
#define TIM4_CNTR	*(unsigned char*)0x5346
#define TIM4_PSCR	*(unsigned char*)0x5347
#define TIM4_ARR	*(unsigned char*)0x5348

/* ADC */
#define ADC_DBxR	*(unsigned char*)0x53E0
/* many more registers for 0x53e0 to 0x53f3 */

#define ADC_CSR		*(unsigned char*)0x5400
#define ADC_CR1		*(unsigned char*)0x5401
#define ADC_CR2		*(unsigned char*)0x5402
#define ADC_CR3		*(unsigned char*)0x5403
#define ADC_DRH		*(unsigned char*)0x5404
#define ADC_DRL		*(unsigned char*)0x5405
#define ADC_TDRH	*(unsigned char*)0x5406
#define ADC_TDRL	*(unsigned char*)0x5407
#define ADC_HTRH	*(unsigned char*)0x5408
#define ADC_HTRL	*(unsigned char*)0x5409
#define ADC_LTRH	*(unsigned char*)0x540A
#define ADC_LTRL	*(unsigned char*)0x540B
#define ADC_AWSRH	*(unsigned char*)0x540C
#define ADC_AWSRL	*(unsigned char*)0x540D
#define ADC_AWCRH	*(unsigned char*)0x540E
#define ADC_AWCRL	*(unsigned char*)0x540F
