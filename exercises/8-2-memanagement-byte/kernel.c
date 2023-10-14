#include "os.h"

/*
 * Following functions SHOULD be called ONLY ONE time here,
 * so just declared here ONCE and NOT included in file os.h.
 */
extern void uart_init(void);
extern void page_init(void);

extern void print_memory();

void start_kernel(void)
{
    uart_init();
    uart_puts("Hello, RVOS!\r\n");

    page_init();

    print_memory();
    printf("malloc p1 p2 p3 p4:\n");
    void *p1 = malloc(100);
    void *p2 = malloc(200);
    void *p3 = malloc(300);
    void *p4 = malloc(40000);
    print_memory();

    if (p2) {
        printf("free p2:\n");
        free(p2);
        print_memory();
    }
    if (p1) {
        printf("free p1:\n");
        free(p1);
        print_memory();
    }
    if (p3) {
        printf("free p3:\n");
        free(p3);
        print_memory();
    }
    if (p4) {
        printf("free p4:\n");
        free(p4);
        print_memory();
    }

    while (1) {};
}