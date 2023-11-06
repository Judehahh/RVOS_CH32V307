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
 */
#define GPIOA 0x40010800L
#define UART1 0x40013800L
#define RCC   0x40021000L
#define PFIC  0xE000E000L

#endif /* __PLATFORM_H__ */