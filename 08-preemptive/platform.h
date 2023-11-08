#ifndef __PLATFORM_H__
#define __PLATFORM_H__

/*
 * MemoryMap
 * 0x0000 0000 -- FLASH Start, our code runs from here
 * 0x2000 0000 -- FLASH End / SRAM Start
 * 0x2001 0000 -- SRAM End
 * 0x4001 0800 -- Port A
 * 0x4001 3800 -- USART1
 * 0x4002 1000 -- RCC
 * 0xE000 E000 -- PFIC
 * 0xE000 F000 -- STK (SysTick)
 */
#define GPIOA 0x40010800L
#define UART1 0x40013800L
#define RCC   0x40021000L
#define PFIC  0xE000E000L
#define STK   0xE000F000L

/* RCC REG */
struct rcc_reg {
    volatile uint32_t CTLR;
    volatile uint32_t CFGR0;
    volatile uint32_t INTR;
    volatile uint32_t APB2PRSTR;
    volatile uint32_t APB1PRSTR;
    volatile uint32_t AHBPCENR;
    volatile uint32_t APB2PCENR;
    volatile uint32_t APB1PCENR;
    volatile uint32_t BDCTLR;
    volatile uint32_t RSTSCKR;
    volatile uint32_t AHBRSTR;
    volatile uint32_t CFGR2;
};

#define RCC_REG ((struct rcc_reg *) RCC)

/* GPIO REG */
struct gpio_reg {
    volatile uint32_t CFGLR;
    volatile uint32_t CFGHR;
    volatile uint32_t INDR;
    volatile uint32_t OUTDR;
    volatile uint32_t BSHR;
    volatile uint32_t BCR;
    volatile uint32_t LCKR;
};

#define GPIOA_REG ((struct gpio_reg *) GPIOA)

/* UART REG */
struct uart_reg {
    volatile uint32_t STATR;
    volatile uint32_t DATAR;
    volatile uint32_t BRR;
    volatile uint32_t CTLR1;
    volatile uint32_t CTLR2;
    volatile uint32_t CTLR3;
    volatile uint32_t GPR;
};

#define UART1_REG ((struct uart_reg *) UART1)

/* PFIC REG */
struct pfic_reg{
  volatile const uint32_t ISR[8];
  volatile const uint32_t IPR[8];
  volatile uint32_t ITHRESDR;
  volatile uint32_t RESERVED;
  volatile uint32_t CFGR;
  volatile const uint32_t GISR;
  volatile uint8_t VTFIDR[4];
  uint8_t RESERVED0[12];
  volatile uint32_t VTFADDR[4];
  uint8_t RESERVED1[0x90];
  volatile uint32_t IENR[8];
  uint8_t RESERVED2[0x60];
  volatile uint32_t IRER[8];
  uint8_t RESERVED3[0x60];
  volatile uint32_t IPSR[8];
  uint8_t RESERVED4[0x60];
  volatile uint32_t IPRR[8];
  uint8_t RESERVED5[0x60];
  volatile uint32_t IACTR[8];
  uint8_t RESERVED6[0xE0];
  volatile uint8_t IPRIOR[256];
  uint8_t RESERVED7[0x810];
  volatile uint32_t SCTLR;
};

#define PFIC_REG ((struct pfic_reg *) PFIC)

/* SysTick REG */
struct stk_reg {
    volatile uint32_t CTLR;
    volatile uint32_t SR;
    volatile uint64_t CNT;
    volatile uint64_t CMP;
};

#define STK_REG ((struct stk_reg *) STK)

#define SYSTEM_CORE_CLOCK 48000000

#endif /* __PLATFORM_H__ */