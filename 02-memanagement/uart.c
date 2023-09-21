#include "types.h"
#include "platform.h"

#define RCC_REG(reg)  ((volatile uint32_t *)(RCC + (reg << 2)))
#define GPIO_REG(reg)  ((volatile uint32_t *)(GPIOA + (reg << 2)))
#define UART_REG(reg) ((volatile uint32_t *)(UART1 + (reg << 2)))

typedef enum {
    CTLR = 0,
    CFGR0 = 1,
    INTR = 2,
    APB2PRSTR = 3,
    APB1PRSTR = 4,
    AHBPCENR = 5,
    APB2PCENR = 6,
    APB1PCENR = 7,
    BDCTLR = 8,
    RSTSCKR = 9,
    AHBRSTR = 10,
    CFGR2 = 11,
} RCC_TypeDef;

typedef enum {
    CFGLR = 0,
    CFGHR = 1,
    INDR = 2,
    OUTDR = 3,
    BSHR = 4,
    BCR = 5,
    LCKR = 6,
} GPIO_TypeDef;

typedef enum {
    START = 0,
    DATAR = 1,
    BRR = 2,
    CTLR1 = 3,
    CTLR2 = 4,
    CTLR3 = 5,
    GPR = 6,
} UART_TypeDef;

/*
 * USART STATUS REGISTER (STATR)
 * STATR BIT 5:
 * 0 = Data not yet received.
 * 1 = Data received and can be read.
 * ......
 * STATR BIT 6:
 * 0 = Transmission not yet completed.
 * 1 = Transmission completed.
 * ......
 */
#define STATR_RX_READY (1 << 5)
#define STATR_TX_IDLE  (1 << 7)

#define rcc_read_reg(reg) (*(RCC_REG(reg)))
#define rcc_write_reg(reg, v) (*(RCC_REG(reg)) = (v))

#define gpio_read_reg(reg) (*(GPIO_REG(reg)))
#define gpio_write_reg(reg, v) (*(GPIO_REG(reg)) = (v))

#define uart_read_reg(reg) (*(UART_REG(reg)))
#define uart_write_reg(reg, v) (*(UART_REG(reg)) = (v))

void uart_init()
{
    /* Enable HSI */
    *RCC_REG(CTLR) |= (uint32_t)0x00000001;

    /* PLL multiplied by 12 */
    *RCC_REG(CFGR0) |= (uint32_t)(0x00280000);

    /* Enable PLL */
    *RCC_REG(CTLR) |= (uint32_t)0x01000000;

    /* Wait till PLL is ready */
    while((*RCC_REG(CTLR) & (uint32_t)0x02000000) == 0);

    /* 
     * Select PLL as system clock source
     *
     * HSI = 8MHz
     * PLLSRC = HSI / 2 = 4MHz
     * PLL = PLLSRC * 12 = 48MHz
     * SYSCLK = PLL = 48MHz
     */
    *RCC_REG(CFGR0) &= (uint32_t)((uint32_t)~(0x00000003));
    *RCC_REG(CFGR0) |= (uint32_t)0x00000002;

    /* 
     * Wait till PLL is used as system clock source
     */
    while ((rcc_read_reg(CFGR0) & (uint32_t)0x0000000C) != (uint32_t)0x08);


    /*
     * Enable clock of USART1 and GPIO_PA
     */
    rcc_write_reg(APB2PCENR, (uint32_t)((uint32_t)(1 << 14) | (uint32_t)(1 << 2)));

    /*
     * Setting GPIO PA9 to
     * - Reuse push-pull output mode
     * - Speed 50MHz
     */
    *GPIO_REG(CFGHR) |= (uint32_t)(0x000000B0);

    /* Set baud rate to 57600
     * BaudRate = FCLK / (16 * USARTDIV)
     * USARTDIV = DIV_M + ( DIV_F / 16)
     * - DIV_M: [15:4] of BRR
     * - DIV_F: [3:0] of BRR
     */
    uart_write_reg(BRR, (uint16_t)0x0341);

    /* Enable Tx for UART0 */
    uart_write_reg(CTLR1, (uint16_t)((uint16_t)(1 << 3) | (uint16_t)(1 << 13)));
}

void uart_putc(uint16_t ch)
{
    uart_write_reg(DATAR, (ch & (uint16_t)0x01FF));
    while ((uart_read_reg(START) & STATR_TX_IDLE) == (uint16_t)0);
}

void uart_puts(char *s)
{
    while (*s) {
        uart_putc(*s++);
    }
}
