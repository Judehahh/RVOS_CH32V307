#include "os.h"

extern void trap_vector(void);
extern void uart_irq_handler(void);

void trap_init()
{
    w_mtvec((reg_t)trap_vector);
}

reg_t trap_handler(reg_t epc, reg_t cause)
{
    reg_t return_pc = epc;
    reg_t cause_code = cause & 0xfff;

    if (cause & 0x80000000) {
        switch (cause_code) {
            case 53:
                uart_puts("USART1 interruption!\n");
                uart_irq_handler();
                break;
            default:
                printf("unkown interruption! code = %d\n", cause_code);
                break;
        }
    } else {
        printf("exception! code = %d\n", cause_code);
        panic("OOPS! What can I do!");
        //return_pc += 4;
    }

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