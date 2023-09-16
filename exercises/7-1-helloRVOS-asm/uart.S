    .global uart_init
    .global uart_putc
    .global uart_puts

    .text
uart_init:
    add     sp, sp, -16
    sw      s0, 12(sp)
    add     s0, sp, 16

    # Enable HSI
    li      t1, 0x40021000
    lw      t0, 0(t1)
    ori     t0, t0, 1
    sw      t0, 0(t1)

    # PLL multiplied by 12
    li      t1, 0x40021004
    li      t2, 0x00280000
    lw      t0, 0(t1)
    or      t0, t0, t2
    sw      t0, 0(t1)

    # Enable PLL
    li      t1, 0x40021000
    li      t2, 0x01000000
    lw      t0, 0(t1)
    or      t0, t0, t2
    sw      t0, 0(t1)

    # Wait till PLL is ready
loop1:
    li      t1, 0x40021000
    li      t2, 0x02000000
    lw      t0, 0(t1)
    and     t0, t0, t2
    beqz    t0, loop1

    # Select PLL as system clock source
    li      t1, 0x40021004
    li      t2, 0x00000003
    not     t2, t2
    lw      t0, 0(t1)
    and     t0, t0, t2
    li      t2, 0x00000002
    or      t0, t0, t2
    sw      t0, 0(t1)

    # Wait till PLL is used as system clock source
loop2:
    li      t1, 0x40021004
    li      t2, 0x0000000C
    lw      t0, 0(t1)
    and     t0, t0, t2
    li      t1, 0x00000008
    bne     t0, t1, loop2

    # Enable clock of USART1 and GPIO_PA
    li      t1, 0x40021018
    li      t0, 0x00004004
    sw      t0, 0(t1)

    # Setting GPIO PA9 to
    # - Reuse push-pull output mode
    # - Speed 50MHz
    li      t1, 0x40010804
    li      t2, 0x000000B0
    lw      t0, 0(t1)
    or      t0, t0, t2
    sw      t0, 0(t1)

    # set baud rate to 57600
    li      t1, 0x40013808
    li      t0, 0x0341
    sw      t0, 0(t1)

    li      t1, 0x4001380c
    li      t0, 0x2008
    sw      t0, 0(t1)

    # ret
    lw      s0, 12(sp)
    add     sp, sp, 16
    ret

uart_putc:
    add     sp, sp, -32
    sw      s0, 28(sp)
    add     s0, sp, 32
    
    # write char to reg
    mv      t0, a0
    li      t1, 0x40013804
    andi    t0, t0, 0x01FF
    sw      t0, 0(t1)

loop3:
    li      t0, 0x40013800
    lw      t0, 0(t0)
    and     t0, t0, 0x80
    beqz    t0, loop3

    # ret
    lw      s0, 28(sp)
    add     sp, sp, 32
    ret

uart_puts:
    add     sp, sp, -32
    sw      ra, 28(sp)
    sw      s0, 24(sp)
    add     s0, sp, 32

    mv      s1, a0
1:
    lbu     a0, 0(s1)
    beqz    a0, 2f      # If end of string, jump to label 2
    call    uart_putc
    addi    s1, s1, 1
    j       1b          # Jump back to label 1 to continue the loop

    # ret
2:
    lw      ra, 28(sp)
    lw      s0, 24(sp)
    add     sp, sp, 32
    ret