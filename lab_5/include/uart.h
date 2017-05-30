#ifndef INCLUDE_UART_H_
#define INCLUDE_UART_H_

#include <msp430.h>
#include <stdint.h>

uint8_t uart_init();

uint8_t uart_getchar();

uint8_t uart_putchar();

int uart_puts(const char *str);
#endif /* INCLUDE_UART_H_ */
