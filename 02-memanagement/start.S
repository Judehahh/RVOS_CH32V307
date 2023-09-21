    .global _start

    .text

_start:
    la      sp, _stack_end
    
    j       start_kernel    # jump to start_kernel
    
    .end