
#include "menu.h"
#include "uart.h"
#include <stdint.h>
#include <stddef.h>
#include <string.h>

static const struct menu_item *_current_menu = NULL;
static size_t _current_menu_size = 0;

static void display_menu(void);

/**
 * \brief Initialize and display the current menu
 * \param[in] menu - array of menu items
 * \param[in] count - number of menu items (max 9)
 */
void menu_init()
{
    display_menu();
}

/**
 * \brief Read user input and execute menu selection
 */
void menu_run(void)
{
    static unsigned int value = 0;
    int c = uart_getchar();

    if ((c >= '0') && (c <= '9')) {
        value *= 10;
        value += c - '0';
        uart_putchar(c);
    } else if ((c == '\n') || (c == '\r')) {    
      uart_puts("\nInvalid selection\n");        
      display_menu();
    } else {
       /* Not a valid character */ 
    }
}


/**
 * \brief Read an unsigned integer from the menu prompt
 * \param[in] prompt - the text to display
 * \return the unsigned integer read 
 */
unsigned int menu_read_uint(const char *prompt)
{
    unsigned int value = 0;
 
    uart_puts(prompt);
    
    while (1) {
        int c = uart_getchar();
     
        if ((c >= '0') && (c <= '9')) {
            // konwersja chara na uinta
            value *= 10;
            value += c - '0';
            uart_putchar(c);
        } else if ((c == '\n') || (c == '\r')) {
            uart_puts("\n");
            break;
        } else {
            /* Not a valid character */
        }
    }

    return value;
}

static void display_menu(void)
{

    uart_puts("\nMenu selection:");
    uart_puts("\nKorona kielce kurwa widelce:");


    /* Print prompt */
    uart_puts("\n> ");
}
