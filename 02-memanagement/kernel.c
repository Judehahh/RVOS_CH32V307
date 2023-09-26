#include "os.h"

extern void uart_init(void);
extern void page_init(void);

int global_init = 0x11111111;
const int global_const = 0x22222222;

void start_kernel(void)
{
    uart_init();
    uart_puts("Hello, RVOS!\r\n");

    page_init();

    static int static_var = 0x33333333;
    static int static_var_uninit;
    int auto_var = 0x44444444;

    printf("addr of global_init = %p\n", &global_init);
    printf("addr of global_const = %p\n", &global_const);
    printf("addr of static_var = %p\n", &static_var);
    printf("addr of static_var_uninit = %p\n", &static_var_uninit);
    printf("addr of auto_var = %p\n", &auto_var);

    while (1) {};
}