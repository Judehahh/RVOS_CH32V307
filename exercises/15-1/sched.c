#include "os.h"

/* defined in entry.S */
extern void switch_to(struct context *next);

#define MAX_TASKS 5
#define STACK_SIZE 2048

/*
 * In the standard RISC-V calling convention, the stack pointer sp
 * is always 16-byte aligned.
 */
uint8_t __attribute__((aligned(16))) task_stack[MAX_TASKS][STACK_SIZE];
struct context ctx_tasks[MAX_TASKS];

/*
 * _top is used to mark the max available position of ctx_tasks
 * _current is used to point to the context of current task
 */
static int _top = 0;
static int _current = -1;

void sched_init()
{
    w_mscratch(0);
}

/*
 * implement a simple cycle FIFO schedular
 */
void schedule()
{
    if (_top <= 0) {
        panic("Num of task should be greater than zero!");
        return;
    }

    _current = (_current + 1) % _top;
    struct context *next = &ctx_tasks[_current];
    switch_to(next);
}

/*
 * DESCRIPTION
 *  Create a task.
 *  - start_routin: task routine entry
 * RETURN VALUE
 *  0: success
 *  -1: if error occurred
 */
int task_create(void (*start_routin)(void))
{
    if (_top < MAX_TASKS) {
        ctx_tasks[_top].sp = (reg_t)&task_stack[_top][STACK_SIZE];
        ctx_tasks[_top].pc = (reg_t)start_routin;
        _top++;
        return 0;
    } else {
        printf("Maximum number of tasks reached. Task creation failed.");
        return -1;
    }
}

/*
 * DESCRIPTION
 *  task_yield() allows the currently executing task to relinquish the CPU
 *  and let system scheduler to select a new task for execution.
 */
void task_yield()
{
    /* trigger a machine-level software interrupt */
    STK_REG->CTLR |= (uint32_t)(1 << 31);
}

/*
 * a very rough implementaion, just to consume the cpu
 */
void task_delay(volatile int count)
{
    count *=2000;
    while (count--);
}