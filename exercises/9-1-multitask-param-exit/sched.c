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
struct task ctx_tasks[MAX_TASKS];

/*
 * _top is used to mark the max available position of ctx_tasks
 * _current is used to point to the context of current task
 */
static int _top = 0;
static int _current = -1;

static void w_mscratch(reg_t x)
{
    asm volatile("csrw mscratch, %0"
                :
                :
                "r" (x));
}

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

    /* find the not exit task with the highest priority */
    int highest = 256;
    int _index = (_current + 1) % _top;
    int _prior = (_current + _top) % _top;
    while (_index != _prior) {
        if (!ctx_tasks[_index].is_exit && ctx_tasks[_index].priority < highest) {
            highest = ctx_tasks[_index].priority;
            _current = _index;
        }
        _index = (_index + 1) % _top;
    }

    struct context *next = &ctx_tasks[_current].ctx;
    printf("stack pointer in schedule(): %p\n", &next);
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
int task_create(void (*start_routin)(void *param), void *param, int priority)
{
    if (_top < MAX_TASKS) {
        ctx_tasks[_top].ctx.sp = (reg_t)&task_stack[_top][STACK_SIZE];
        ctx_tasks[_top].ctx.ra = (reg_t)start_routin;
        ctx_tasks[_top].priority = priority;
        ctx_tasks[_top].is_exit = 0;
        if (param) {
            ctx_tasks[_top].ctx.a0 = (reg_t)param;
        }
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
    schedule();
}

/*
 * a very rough implementaion, just to consume the cpu
 */
void task_delay(volatile int count)
{
    count *=10000;
    while (count--);
}

void task_exit()
{
    ctx_tasks[_current].is_exit = 1;
    schedule();
}