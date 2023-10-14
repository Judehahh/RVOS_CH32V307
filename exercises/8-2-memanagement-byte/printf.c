#include "os.h"

/*
 * ref: https://github.com/cccriscv/mini-riscv-os/blob/master/05-Preemptive/lib.c
 */

static int _vsnprintf(char *out, size_t n, const char *s, va_list v1) {
    size_t pos = 0;
    int is_format = 0;
    int is_long = 0;

    for (; *s; s++) {
        if (is_format) {
            switch (*s) {
                case 'l': {
                    is_long = 1;
                    break;
                }
                case 'p': {
                    is_long = 1;
                    if (out && pos < n) {
                        out[pos] = '0';
                    }
                    ++pos;
                    if (out && pos < n) {
                        out[pos] = 'x';
                    }
                    ++pos;
                }   // don't break here!!!
                case 'x': {
                    long num = is_long ? va_arg(v1, long) : va_arg(v1, int);
                    int hex_digits = 2 * (is_long ? sizeof(long) : sizeof(int)); // two hex numbers in one byte
                    for (int i = hex_digits - 1; i >= 0; i--) {
                        int digit = (num >> (i * 4)) & 0xF;
                        if (out && pos < n) {
                            out[pos] = digit < 10 ? '0' + digit : 'a' + digit - 10;
                        }
                        ++pos;
                    }
                    is_format = 0;
                    is_long = 0;
                    break;
                }
                case 'X': {
                    long num = is_long ? va_arg(v1, long) : va_arg(v1, int);
                    int hex_digits = 2 * (is_long ? sizeof(long) : sizeof(int)); // two hex numbers in one byte
                    for (int i = hex_digits - 1; i >= 0; i--) {
                        int digit = (num >> (i * 4)) & 0xF;
                        if (out && pos < n) {
                            out[pos] = digit < 10 ? '0' + digit : 'A' + digit - 10;
                        }
                        ++pos;
                    }
                    is_format = 0;
                    is_long = 0;
                    break;
                }
                case 'd': {
                    long num = is_long ? va_arg(v1, long) : va_arg(v1, int);
                    if (num < 0) {
                        num = -num;
                        if (out && pos < n) {
                            out[pos] = '-';
                        }
                        ++pos;
                    }
                    int digits = 1;
                    for (int nn = num; nn /= 10; ++digits);
                    for (int i = digits - 1; i >= 0; --i) {
                        if (out && pos + i < n) {
                            out[pos + i] = '0' + (num % 10);
                        }
                        num /= 10;
                    }
                    pos += digits;
                    is_format = 0;
                    is_long = 0;
                    break;
                }
                case 'c': {
                    if (out && pos < n) {
                        out[pos] = (char)va_arg(v1, int);
                    }
                    ++pos;
                    is_format = 0;
                    is_long = 0;
                    break;
                }
                case 's': {
                    const char *str = va_arg(v1, const char *);
                    while (*str) {
                        if (out && pos < n) {
                            out[pos] = *str;
                        }
                        ++str;
                        ++pos;
                    }
                    is_format = 0;
                    is_long = 0;
                    break;
                }
                default:
                    break;
            }
        }
        else if (*s == '%') {
            is_format = 1;
        } else {
            if (out && pos < n) {
                out[pos] = *s;
            }
            ++pos;
        }
        if (out && pos < n) {
            out[pos] = 0;
        }
    }

    return pos;
}

static char out_buf[1024];  // buffer for _vprintf

static int _vprintf(const char *s, va_list v1)
{
    int len = 0;
    len = _vsnprintf(NULL, -1, s, v1);
    if (len > sizeof(out_buf) - 1) {
        uart_puts("[ERROR] _vprintf(): output string size overflow.\r\n");
        while (1) {}
    }
    _vsnprintf(out_buf, len + 1, s, v1);    // add 1 for '\0'
    uart_puts(out_buf);
    return len;
}

int printf(const char *s, ...)
{
    int ret = 0;
    va_list v1;             // pointer to the variable argument list

    va_start(v1, s);        // point v1 to the first element of s
    ret = _vprintf(s, v1);
    va_end(v1);             // release v1

    return ret;
}