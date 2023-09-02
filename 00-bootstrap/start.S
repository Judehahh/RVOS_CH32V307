#include "platform.h"

    .equ    STACK_SIZE, 1024

    .global _start

    .text

_start:
    add     t0, x0, 0
    slli    t0, t0, 10
    la      sp, stacks + STACK_SIZE
    add     sp, sp, t0
    
    j       start_kernel    # jump to start_kernel
    
.align 16
stacks:
    .skip   STACK_SIZE * MAXNUM_CPU

    .end