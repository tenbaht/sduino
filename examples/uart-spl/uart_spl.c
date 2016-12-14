#include"stm8s.h"

void send_string(char *str)
{
    char c;

    if (!str) return;

    while ( c=*str++ ) {	// assignment intented
        while (!UART1_GetFlagStatus(UART1_FLAG_TXE));
        UART1_SendData8(c);
    }
}


void main (void)
{
    uint32_t i;

    UART1_DeInit();
    UART1_Init(9600, UART1_WORDLENGTH_8D, UART1_STOPBITS_1, UART1_PARITY_NO,
        UART1_SYNCMODE_CLOCK_DISABLE, UART1_MODE_TXRX_ENABLE);

    while (1) {
        send_string("Hello World!\n");
        for (i=30000; i; i--);
    }
}
