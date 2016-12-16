/*
 * test HardwareSerial.c using spl functions
 * no dependencies to wiring
 */

#include "stm8s.h"
#include "HardwareSerial.h"

void UART1_RX_IRQHandler(void) __interrupt(ITC_IRQ_UART1_RX); /* UART1 RX */
void UART1_TX_IRQHandler(void) __interrupt(ITC_IRQ_UART1_TX); /* UART1 TX */



void send_string(char *str)
{
    char c;

    if (!str) return;

    while ( c=*str++ ) HardwareSerial_write(c);	// assignment intented
}


void main (void)
{
    uint32_t i;

    UART1_DeInit();
    enableInterrupts();

    HardwareSerial_begin(9600);

    while (1) {
        send_string("Hello World!\r\n");
        for (i=15000; i; i--);
        while(HardwareSerial_available()) {
            HardwareSerial_write('.');
            HardwareSerial_write(HardwareSerial_read());
        };
        for (i=15000; i; i--);
    }
}
