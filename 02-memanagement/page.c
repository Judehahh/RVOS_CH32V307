#include "os.h"

/*
 * Following global vars are defined in mem.S
 */
extern uint32_t TEXT_START;
extern uint32_t TEXT_END;
extern uint32_t DATA_START;
extern uint32_t DATA_END;
extern uint32_t RODATA_START;
extern uint32_t RODATA_END;
extern uint32_t BSS_START;
extern uint32_t BSS_END;
extern uint32_t HEAP_START;
extern uint32_t HEAP_SIZE;
extern uint32_t STACK_START;
extern uint32_t STACK_END;

void page_init()
{
    printf("HEAP_START = %x, HEAP_SIZE = %x\n", HEAP_START, HEAP_SIZE);

    printf("TEXT:   0x%x -> 0x%x\n", TEXT_START, TEXT_END);
    printf("RODATA: 0x%x -> 0x%x\n", RODATA_START, RODATA_END);
    printf("DATA:   0x%x -> 0x%x\n", DATA_START, DATA_END);
    printf("BSS:    0x%x -> 0x%x\n", BSS_START, BSS_END);
    printf("STACK:  0x%x -> 0x%x\n", STACK_START, STACK_END);
}