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
extern void uart_printf(char *fmt, ...);

/* memory management */
extern void *page_alloc(int npages);
extern void page_free(void *p);
extern void *malloc(uint32_t nbytes);
extern void free(void *ap);

#endif /* __OS_H__ */