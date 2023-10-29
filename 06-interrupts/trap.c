#include "os.h"

extern void trap_vector(void);

void trap_init()
{
    w_mtvec((reg_t)trap_vector);
}

reg_t trap_handler(reg_t epc, reg_t cause)
{
    reg_t return_pc = epc;
    reg_t cause_code = cause;

    if (cause & 0x80000000) {
        printf("interruption!\n");
    } else {
        printf("exception! code = %d\n", cause_code);
    }
    panic("go into trap_handler!");

    return return_pc;
}

void trap_test()
{
    /*
     * Synchronous exception code = 4
     * Load address misaligned
     */
    int a = *(int *)0xFFFFFFF;

    /*
     * Synchronous exception code = 5
     * Load access fault
     */
    // int a = *(int *)0xFFFFFFC;
    printf("a = %d\n", a);

    uart_puts("Yeah! I'm return back from trap!\n");
}