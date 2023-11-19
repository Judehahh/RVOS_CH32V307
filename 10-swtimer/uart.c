#include "os.h"

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

void uart_init()
{
    /*
     * Enable clock of USART1 and GPIO_PA
     */
    RCC_REG->APB2PCENR = (uint32_t)((uint32_t)(1 << 14) | (uint32_t)(1 << 2));

    /*
     * Setting GPIO PA9 to
     * - Reuse push-pull output mode
     * - Speed 50MHz
     */
    GPIOA_REG->CFGHR |= (uint32_t)(0x000000B0);

    /* Set baud rate to 57600
     * BaudRate = FCLK / (16 * USARTDIV)
     * USARTDIV = DIV_M + ( DIV_F / 16)
     * - DIV_M: [15:4] of BRR
     * - DIV_F: [3:0] of BRR
     */
    UART1_REG->BRR = (uint16_t)0x0341;

    /* Enable Rx for UART1 */
    UART1_REG->CTLR1 |= (uint16_t)(1 << 2);
    /* Enable Tx for UART1 */
    UART1_REG->CTLR1 |= (uint16_t)(1 << 3);
    /* Enable RXNE(RX buff Not Empty) interrupt for UART1 */
    UART1_REG->CTLR1 |= (uint16_t)(1 << 5);
    /* Enable UART1 */
    UART1_REG->CTLR1 |= (uint16_t)(1 << 13);
}

void uart_putc(uint16_t ch)
{
    UART1_REG->DATAR = (ch & (uint16_t)0x01FF);
    while ((UART1_REG->STATR & STATR_TX_IDLE) == (uint16_t)0);
}

void uart_puts(char *s)
{
    while (*s) {
        uart_putc(*s++);
    }
}

int uart_getc(void)
{
    if (UART1_REG->STATR & STATR_RX_READY) {
        return (uint16_t)(UART1_REG->DATAR & (uint16_t)0x01FF);
    } else {
        return -1;
    }
}

/*
 * handle a uart interrupt, raised because input has arrived, called from trap.c.
 */
void uart_irq_handler(void)
{
	while (1) {
		int c = uart_getc();
		if (c == -1) {
			break;
		} else {
			uart_putc((char)c);
			uart_putc('\n');
		}
	}
}
