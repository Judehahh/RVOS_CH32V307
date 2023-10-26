#include "os.h"

#define DELAY 1000

void user_task0(void *param)
{
    int count = *(int *)param;
    uart_puts("Task 0: Created!\n");
    while (count) {
        printf("Task 0: count = %d\n", count--);
        task_delay(DELAY);
        task_yield();
    }
    task_exit();
}

void user_task1(void *param)
{
    int count = *(int *)param;
    uart_puts("Task 1: Created!\n");
    while (count) {
        printf("Task 1: count = %d\n", count--);
        task_delay(DELAY);
        task_yield();
    }
    task_exit();
}

void user_task2(void *param)
{
    uart_puts("Task 2: Created!\n");
    while (1) {
        uart_puts("Task 2: Running...\n");
        task_delay(DELAY);
        task_yield();
    }
}

void os_main(void)
{
    int count0 = 10;
    int count1 = 5;
    task_create(user_task0, (void *)&count0, 10);
    task_create(user_task1, (void *)&count1, 10);
    task_create(user_task2, NULL, 20);
    /*
     * move schedule() to here,
     * otherwise the stack containing
     * count0 and count1 will be reclaimed.
     */
    schedule();
}