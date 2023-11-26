#include "os.h"
#include "user_api.h"

#define DELAY 2000

void user_task0(void)
{
    uart_puts("Task 0: Created!\n");

    unsigned int archid = -1;

    /*
     * if syscall is supported, this will trigger exception, 
     * code = 2 (Illegal instruction)
     */
    // archid = r_marchid();
    // printf("hart id is %d\n", archid);

#ifdef CONFIG_SYSCALL
    int ret = -1;
    ret = getarchid(&archid);
    // ret = getarchid(NULL);
    if (ret) {
        printf("getarchid() failed, return: %d\n", ret);
    } else {
        printf("system call returned, marchid id 0x%X\n", archid);
    }
#endif

    while (1) {
        uart_puts("Task 0: Running...\n");
        task_delay(DELAY);
    }
}

void user_task1(void)
{
    uart_puts("Task 1: Created!\n");
    while (1) {
        uart_puts("Task 1: Running...\n");
        task_delay(DELAY);
    }
}

void os_main(void)
{
    task_create(user_task0);
    task_create(user_task1);
}
