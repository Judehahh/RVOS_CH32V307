#include "os.h"

extern void trap_vector(void);
extern void uart_irq_handler(void);
extern void timer_handler(void);
extern void do_syscall(struct context *cxt);

void trap_init()
{
    w_mtvec((reg_t)trap_vector);
}

reg_t trap_handler(reg_t epc, reg_t cause, struct context *cxt)
{
    reg_t return_pc = epc;
    reg_t cause_code = cause & 0xfff;

    if (cause & 0x80000000) {
        switch (cause_code) {
            case 12:
                uart_puts("SysTick interruption!\n");
                timer_handler();
                break;
            case 14:
                uart_puts("Software interruption!\n");
                /* Clear the software interrupt bit */
                STK_REG->CTLR &= ~((uint32_t)(1 << 31));
                schedule();
                break;
            case 53:
                uart_puts("USART1 interruption!\n");
                uart_irq_handler();
                break;
            default:
                printf("unkown interruption! code = %d\n", cause_code);
                break;
        }
    } else {
        switch (cause) {
            case 8:
                uart_puts("System call from U-mode!\n");
                do_syscall(cxt);
                return_pc += 4;
                break;
            default:
                printf("exception! code = %d\n", cause_code);
                panic("OOPS! What can I do!");
                // return_pc += 4;
        }
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