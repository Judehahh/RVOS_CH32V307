#include "os.h"

#define PFIC_IENR2 0xE000E104L

void pfic_init(void)
{
    /*
     * At the end of start_kernel, schedule() will call MRET to switch
     * to the first task, so we parepare the mstatus here.
     * Notice: default mstatus is 0
     * Set mstatus.MPP to 3, so we still run in Machine mode after MRET.
     * Set mstatus.MPIE to 1, so MRET will enable the interrupt.
     */
    w_mstatus(MSTATUS_MPP | MSTATUS_MPIE | MSTATUS_MIE);
    
    /* enable SysTick & software interrupts. */
    PFIC_REG->IENR[0] |= (uint32_t)(0x00005000);

    /* enable usart1 interrupts. */
    PFIC_REG->IENR[1] |= (uint32_t)(0x00200000);
}