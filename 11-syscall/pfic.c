#include "os.h"

#define PFIC_IENR2 0xE000E104L

void pfic_init(void)
{
    /* enable machine-mode external interrupts. */
    w_mstatus(r_mstatus() | MSTATUS_MIE);
    
    /* enable SysTick & software interrupts. */
    PFIC_REG->IENR[0] |= (uint32_t)(0x00005000);

    /* enable usart1 interrupts. */
    PFIC_REG->IENR[1] |= (uint32_t)(0x00200000);
}