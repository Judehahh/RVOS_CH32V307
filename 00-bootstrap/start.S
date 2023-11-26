    .global _start

    .text

_start:
    la      sp, 0x20010000  # set stack pointer to the end of SRAM

    j       start_kernel    # jump to start_kernel

    .end
