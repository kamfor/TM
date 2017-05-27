#include <msp430.h> 
#include "include/uart.h"
#include "include/system.h"
#include "include/watchdog.h"

volatile uint8_t char_buffor = 0;


int main(void) {
	watchdog_disable();
    system_init();
    uart_init();

    for (;;)
    {
    	//loop
    }

	return 0;
}


// USART Rx interrupt service routine
#pragma vector=USART0RX_VECTOR
__interrupt void USART_TX_ISR (void)
{
	U1TXBUF = (uint8_t) char_buffor;

}

// USART Tx interrupt service routine
#pragma vector=USART0TX_VECTOR
__interrupt void Usart_Tx_ISR (void)
{
	char_buffor = (uint8_t) U1RXBUF;
}

