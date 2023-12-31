#include "os.h"

#define PFIC_IENR2 0xE000E104L

void pfic_init(void)
{
    /* enable machine-mode global interrupts. */
    w_mstatus(MSTATUS_MPIE | MSTATUS_MIE);

    /* enable usart1 interrupts. */
    *(uint32_t *)PFIC_IENR2 |= (uint32_t)(0x00200000);
}