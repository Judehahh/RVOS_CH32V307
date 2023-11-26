#include "syscall.h"

.global getarchid
getarchid:
    li  a7, SYS_getarchid
    ecall
    ret