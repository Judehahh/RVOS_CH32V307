#include "os.h"
#include "syscall.h"

int sys_getarchid(unsigned int *ptr_archid)
{
    printf("--> sys_getarchid, arg0 = 0x%x\n", ptr_archid);
    if(ptr_archid == NULL) {
        return -1;
    }

    *ptr_archid = r_marchid();
    return 0;
}

void do_syscall(struct context *cxt)
{
    uint32_t syscall_num = cxt->a7;

    switch (syscall_num)
    {
    case SYS_getarchid:
        cxt->a0 = sys_getarchid((unsigned int *)cxt->a0);
        break;
    default:
        printf("Unknown syscall no: %d\n", syscall_num);
        cxt->a0 = -1;
    }

    return;
}