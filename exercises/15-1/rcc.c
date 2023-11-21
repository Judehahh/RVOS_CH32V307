#include "os.h"

void rcc_init()
{
    /* Enable HSI */
    RCC_REG->CTLR |= (uint32_t)0x00000001;

    /* PLL multiplied by 12 */
    RCC_REG->CFGR0 |= (uint32_t)(0x00280000);

    /* Enable PLL */
    RCC_REG->CTLR |= (uint32_t)0x01000000;

    /* Wait till PLL is ready */
    while((RCC_REG->CTLR & (uint32_t)0x02000000) == 0);

    /* 
     * Select PLL as system clock source
     *
     * HSI = 8MHz
     * PLLSRC = HSI / 2 = 4MHz
     * PLL = PLLSRC * 12 = 48MHz
     * SYSCLK = PLL = 48MHz
     */
    RCC_REG->CFGR0 &= (uint32_t)((uint32_t)~(0x00000003));
    RCC_REG->CFGR0 |= (uint32_t)0x00000002;

    /* 
     * Wait till PLL is used as system clock source
     */
    while ((RCC_REG->CFGR0 & (uint32_t)0x0000000C) != (uint32_t)0x08);
}