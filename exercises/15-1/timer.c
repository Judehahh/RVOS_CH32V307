#include "os.h"

static uint8_t  p_us = 0;
static uint16_t p_ms = 0;

static uint32_t _tick = 0;

#define MAX_TIMER 10
static struct timer timer_list[MAX_TIMER];

void timer_init()
{
    struct timer *t = timer_list;
    for (int i = 0; i < MAX_TIMER; i++) {
        t->func = NULL;     // use .func to flag if the item is used
        t->arg = NULL;
        t++;
    }

    p_us = SYSTEM_CORE_CLOCK / 8000000;
    p_ms = (uint16_t)p_us * 1000;

    /* Set CMP register to 1 second */
    STK_REG->CMP = (uint64_t)(p_ms * 1000);

    /* Enable SysTick */
    STK_REG->CTLR |= (uint32_t)(1 << 0);
    /* Enable SysTick interrupts */
    STK_REG->CTLR |= (uint32_t)(1 << 1);
    /* Clear the count value to 0 */
    STK_REG->CTLR |= (uint32_t)(1 << 5);

    /* Clear the status register flag bit */
    STK_REG->SR &= ~(1 << 0);
}

struct timer *timer_create(void (*handler)(void *arg), void *arg, uint32_t timeout)
{
    /* TBD: params should be checked more, but now we just simplify this */
    if (handler == NULL || timeout == 0) {
        return NULL;
    }

    /* use lock to protect the shared timer_list between multiple tasks */
    spin_lock();

    struct timer *t = timer_list;

    int i = 0;
    for (; i < MAX_TIMER; i++) {
        if (t->func == NULL) {
            break;
        }
        t++;
    }

    if (i >= MAX_TIMER) {
        spin_unlock();
        return NULL;
    }

    t->func = handler;
    t->arg = arg;
    t->timeout_tick = timeout;

    spin_unlock();

    return t;
}

void timer_delete(struct timer *timer)
{
    spin_lock();

    struct timer *t = timer_list;
    for (int i = 0; i < MAX_TIMER; i++) {
        if (t == timer) {
            t->func = NULL;
            t->arg = NULL;
            break;
        }
        t++;
    }

    spin_unlock();
}

/* this routine should be called in interrupt context (interrupt is disabled) */
static inline void timer_check()
{
    struct timer *t = timer_list;
    for (int i = 0; i < MAX_TIMER; i++) {
        if (t->func) {
            if (_tick >= t->timeout_tick) {
                t->func(t->arg);

                /* once time, just delete it after timeout */
                t->func = NULL;
                t->arg = NULL;

                // break;
            }
        }
        t++;
    }
}

void timer_handler()
{
    _tick++;
    printf("tick: %d\n", _tick);

    timer_check();

    STK_REG->CTLR |= (uint32_t)(1 << 5);
    STK_REG->SR &= ~(1 << 0);

    schedule();
}
