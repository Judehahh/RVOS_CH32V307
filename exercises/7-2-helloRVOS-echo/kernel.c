#include "types.h"

extern void uart_init(void);
extern void uart_putc(uint16_t ch);
extern void uart_puts(char *s);
extern uint16_t uart_getc();

void start_kernel(void)
{
    uart_init();
    uart_puts("Hello, RVOS!\r\n");

    while (1) {
        uint16_t ch = uart_getc();
        switch(ch) {
            case '\r' :
                uart_puts("\r\n");
                break;
            case '\b' :
            case 127 :
                uart_puts("\b \b");
                break;
            default :
                uart_putc(ch);
        }
    };
}