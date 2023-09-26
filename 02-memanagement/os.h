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

#endif /* __OS_H__ */