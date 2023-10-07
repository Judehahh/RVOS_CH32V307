#include "os.h"

/*
 * Following functions SHOULD be called ONLY ONE time here,
 * so just declared here ONCE and NOT included in file os.h.
 */
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

    void *p = page_alloc(2);
    printf("p = %p\n", p);
    page_free(p);
    printf("free p\n");

    void *p2 = page_alloc(3);
    printf("p2 = %p\n", p2);

    void *p3 = page_alloc(4);
    printf("p3 = %p\n", p3);

    while (1) {};
}