    .global _start

    .text

_start:
    la      sp, _stack_end

    # load data section from flash to RAM
    la      a0, _data_lma
    la      a1, _data_vma
    la      a2, _data_end
    bgeu    a1, a2, 2f
1:
    lw      t0, (a0)
    sw      t0, (a1)
    addi    a0, a0, 4
    addi    a1, a1, 4
    bltu    a1, a2, 1b
2:
    # clear bss section
    la      a0, _bss_start
    la      a1, _bss_end
    bgeu    a0, a1, 2f
1:
    sw      zero, (a0)
    addi    a0, a0, 4
    bltu    a0, a1, 1b
2:
    # At the end of start_kernel, schedule() will call MRET to switch
    # to the first task, so we parepare the mstatus here.
    # Notice: default mstatus is 0
    # Set mstatus.MPP to 3, so we still run in Machine mode after MRET.
    # Set mstatus.MPIE to 1, so MRET will enable the interrupt.
    li      t0, 3 << 11 | 1 << 7
    csrr    a1, mstatus
    or      t0, t0, a1
    csrw    mstatus, t0

    # jump to start_kernel
    j       start_kernel

    .end
