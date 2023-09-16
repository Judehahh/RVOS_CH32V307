    .global start_kernel

    .section .rodata
# The string to be printed
myString:
    .asciz "Hello, RVOS in asm!\n"

    .text
# The start of the kernel
start_kernel:
    add     sp, sp, -16
    sw      ra, 12(sp)
    sw      s0, 8(sp)
    add     s0, sp, 16

    # Initialize the UART
    call    uart_init

    # Print the string
    la      a0, myString
    call    uart_puts

# The stop label for the infinite loop
stop:
    nop
    j       stop

.end
