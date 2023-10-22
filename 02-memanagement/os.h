#ifndef __OS_H__
#define __OS_H__

#include "types.h"
#include "platform.h"

#include <stddef.h>
#include <stdarg.h>

/* uart */
extern void uart_putc(uint16_t ch);
extern void uart_puts(char *s);

/* printf */
extern int printf(const char *s, ...);
extern void panic(char *s);

/* memory management */
extern void *page_alloc(int npages);
extern void page_free(void *p);

#endif /* __OS_H__ */