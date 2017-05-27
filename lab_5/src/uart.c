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



	return status;
}

uint8_t uart_getchar()
{
	int status = -1;

	return status;
}

uint8_t uart_putchar()
{
	int status = -1;

	return status;
}
