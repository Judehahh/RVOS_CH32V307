#include "os.h"

#define PFIC_IENR2 0xE000E104L

void pfic_init(void)
{
    /* enable machine-mode global interrupts. */
    w_mstatus(r_mstatus() | MSTATUS_MPIE | MSTATUS_MIE);

    /* enable SysTick interrupts. */
    PFIC_REG->IENR[0] |= (uint32_t)(0x00001000);

    /* enable usart1 interrupts. */
    PFIC_REG->IENR[1] |= (uint32_t)(0x00200000);
}