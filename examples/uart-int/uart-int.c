#include"stm8s.h"

volatile char	buf=0;
volatile char	transmitting=0;

//void TIM4_ISR(void) __interrupt(23);
//INTERRUPT void UART1_TX_IRQHandler(void); /* UART1 TX */
void UART1_TX_IRQHandler(void) __interrupt(ITC_IRQ_UART1_TX); /* UART1 TX */
//void UART_TX_vector(void) __interrupt(UART1_TX_vector)


void UART1_TX_IRQHandler(void) __interrupt(ITC_IRQ_UART1_TX) /* UART1 TX */
{
    if (buf) {
        // es wartet noch etwas, sofort senden
        UART1_SendData8(buf);
        buf = 0;
    } else {
        // Puffer leergelaufen, Übertragung beenden, Interrupt deaktivieren
        transmitting = 0;
        UART1_ITConfig(UART1_IT_TXE, DISABLE);
    }
}



void send_char(uint8_t val)
{
    if (!transmitting) {
        transmitting = 1;
        UART1_SendData8(val);
        UART1_ITConfig(UART1_IT_TXE, ENABLE);
    } else {
//        while (!UART1_GetFlagStatus(UART1_FLAG_TXE));
        while (buf);
        buf = val;
    }
}


void send_string(char *str)
{
    char c;

    if (!str) return;

    while ( c=*str++ ) send_char(c);	// assignment intented
}


void main (void)
{
    uint32_t i;

    UART1_DeInit();
//    ITC_DeInit();
//    ITC_SetSoftwarePriority(ITC_IRQ_UART1_TX, ITC_PRIORITYLEVEL_2);

//    UART1_ITConfig(UART1_IT_TXE, ENABLE);
    enableInterrupts();

    UART1_Init(9600, UART1_WORDLENGTH_8D, UART1_STOPBITS_1, UART1_PARITY_NO,
        UART1_SYNCMODE_CLOCK_DISABLE, UART1_MODE_TXRX_ENABLE);

    while (1) {
        send_string("Hello World!\n");
        for (i=30000; i; i--);
    }
}
