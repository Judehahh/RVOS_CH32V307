/*
 * os.ld
 * Linker script for outputting to RVOS
 */

/*
 * https://sourceware.org/binutils/docs/ld/Miscellaneous-Commands.html
 * OUTPUT_ARCH command specifies a particular output machine architecture.
 * "riscv" is the name of the architecture for both 64-bit and 32-bit
 * RISC-V target. We will further refine this by using -march=rv32ima
 * and -mabi=ilp32 when calling gcc.
 */
OUTPUT_ARCH( "riscv" )

/*
 * https://sourceware.org/binutils/docs/ld/Entry-Point.html
 * ENTRY command is used to set the "entry point", which is the first instruction
 * to execute in a program.
 * The argument of ENTRY command is a symbol name, here is "_start" which is
 * defined in start.S.
 */
ENTRY( _start )

/*
 * Define stack size
 */
__stack_size = 2048;

PROVIDE( _stack_size = __stack_size );

/* 
 * FLASH + RAM supports the following configuration
 * FLASH-192K + RAM-128K
 * FLASH-224K + RAM-96K
 * FLASH-256K + RAM-64K  
 * FLASH-288K + RAM-32K  
*/
MEMORY {
    FLASH (rx) : ORIGIN = 0x00000000, LENGTH = 256K
    RAM (xrw) : ORIGIN = 0x20000000, LENGTH = 64K
}

SECTIONS {
    /*
     * We are going to layout all text sections in .text output section,
     * starting with .text. The asterisk("*") in front of the
     * parentheses means to match the .text section of ANY object file.
     */
    .text : {
        PROVIDE(_text_start = .);
        *(.text .text.*)
        PROVIDE(_text_end = .);
    } >FLASH AT>FLASH
    
    .rodata : {
        PROVIDE(_rodata_start = .);
        *(.rodata .rodata.*)
        PROVIDE(_rodata_end = .);
    } >FLASH AT>FLASH

    /*
     * .data section is placed in RAM during runtime but in FLASH during linking.
     * The symbol _data_vma equal to the start of the .data section in RAM (VMA).
     * The symbol _data_lma equal to the start of the .data section in FLASH (LMA).
     */
    .dalign : {
        . = ALIGN(4);
        PROVIDE(_data_vma = .);
    } >RAM AT>FLASH	

    .dlalign : {
        . = ALIGN(4); 
        PROVIDE(_data_lma = .);
    } >FLASH AT>FLASH

    .data : {
        PROVIDE(_data_start = .);
        *(.data .data.*)
        *(.sdata .sdata.*)
        . = ALIGN(4);
        PROVIDE(_data_end = .);
    } >RAM AT>FLASH

    .bss : {
        . = ALIGN(4);
        PROVIDE(_bss_start = .);
        *(.sbss .sbss.*)
        *(.bss .bss.*)
        . = ALIGN(4);
        PROVIDE(_bss_end = .);
    } >RAM AT>FLASH

    .stack ORIGIN(RAM) + LENGTH(RAM) - __stack_size : {
        PROVIDE(_heap_end = .);
        . = ALIGN(4);
        PROVIDE(_stack_start = .);
        . = . + __stack_size;
        PROVIDE(_stack_end = .);
    } >RAM

    PROVIDE(_memory_start = ORIGIN(RAM));
    PROVIDE(_memory_end = ORIGIN(RAM) + LENGTH(RAM));

    PROVIDE(_heap_start = _bss_end);
    PROVIDE(_heap_size = _stack_start - _heap_start);
}