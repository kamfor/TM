#include "../include/uart.h"


uint8_t uart_init()
{
	int status = -1;

	/*
	 * http://daycounter.com/Calculators/MSP430-Uart-Calculator.phtml
	 * UART Clock = 7372.8
	 * Baud Rate = 115.2
	 * Number of bits = 11
	 * */
	U0BR0 = 0x40;
	U0BR1 = 0x00;
	U0MCTL = 0x00;
  
  //U0TCTL
  //U0RCTL
  
  IE1 |= URXIE0;
  IE1 |= UTXIE0;


	return status;
}

uint8_t uart_getchar()
{
    int chr = -1;

    if (IFG1 & URXIFG0) {
        chr = U0RXBUF;
    }

    return chr;

}

uint8_t uart_putchar()
{
  
/* Wait for the transmit buffer to be ready */
  while (!(IFG1 & UTXIFG0));

  /* Transmit data */
  U0TXBUF = (char ) c; 
  
  return 0;
}

int uart_puts(const char *str)
{
  int status = -1;

  if (str != NULL) {
    status = 0;

    while (*str != '\0') {
        /* Wait for the transmit buffer to be ready */
        while (!(IFG1 & UTXIFG0));

        /* Transmit data */
        U0TXBUF = *str; 

        /*  If there is a line-feed, add a carriage return */
        if (*str == '\n') {
            /* Wait for the transmit buffer to be ready */
            while (!(IFG1 & U0TXBUF));
            U0TXBUF = '\r';
        }

        str++;
      }
  }

  return status;
}

