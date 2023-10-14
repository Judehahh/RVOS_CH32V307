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

/*
 * _alloc_start points to the actual start address of heap pool
 * _alloc_end points to the actual end address of heap pool
 * _num_pages holds the actual max number of pages we can allocate.
 */
static uint32_t _alloc_start = 0;
static uint32_t _alloc_end = 0;
static uint32_t _num_pages = 0;

#define PAGE_SIZE 4096
#define PAGE_MASK (~(PAGE_SIZE - 1))

#define PAGE_RESERVED 1

#define PAGE_TAKEN (uint8_t)(1 << 0)
#define PAGE_LAST (uint8_t)(1 << 1)

/*
 * Page Descriptor
 * flags:
 * - bit 0: flag if this page is taken(allocated)
 * - bit 1: flag if this page is the last page of the memory block allocated
 */
struct page {
    uint8_t flags;
};

static inline void _clear_page(struct page *page)
{
    page->flags = 0;
}

static inline int _is_page_free(struct page *page)
{
    return !(page->flags & PAGE_TAKEN);
}

static inline void _set_page_flag(struct page *page, uint8_t flag)
{
    page->flags |= flag;
}

static inline int _is_page_last(struct page *page)
{
    return page->flags & PAGE_LAST;
}

/*
 * align the address to the border of page(4K).
 */
static inline uint32_t _align_page(uint32_t addr)
{
    /* 
     * Add PAGE_SIZE - 1 to the address. This increases the address
     * to the next page boundary if it's not already aligned.
     * Then perform a bitwise AND operation with PAGE_MASK to
     * "rounds down" the address to the nearest page boundary.
     */
    return (addr + PAGE_SIZE - 1) & PAGE_MASK;
}

void page_init()
{
    /* 
     * We reserved PAGE_RESERVED pages to hold the Page structures.
     * Sub more 1 to avoid overlapping heap and stack.
    */
    _num_pages = (HEAP_SIZE / PAGE_SIZE) - PAGE_RESERVED - 1;

    printf("HEAP_START = %x, HEAP_SIZE = %x, num of pages = %d\n",
           HEAP_START, HEAP_SIZE, _num_pages);

    struct page *page = (struct page *)HEAP_START;
    for (int i = 0; i < _num_pages; i++) {
        _clear_page(page);
        page++;
    }

    _alloc_start = _align_page(HEAP_START + PAGE_RESERVED * PAGE_SIZE);
    _alloc_end = _alloc_start + _num_pages * PAGE_SIZE;

    printf("TEXT:   0x%x -> 0x%x\n", TEXT_START, TEXT_END);
    printf("RODATA: 0x%x -> 0x%x\n", RODATA_START, RODATA_END);
    printf("DATA:   0x%x -> 0x%x\n", DATA_START, DATA_END);
    printf("BSS:    0x%x -> 0x%x\n", BSS_START, BSS_END);
    printf("HEAP:   0x%x -> 0x%x\n", _alloc_start, _alloc_end);
    printf("STACK:  0x%x -> 0x%x\n", STACK_START, STACK_END);
}

/*
 * Allocate a memory block which is composed of contiguous physical pages
 * - npages: the number of PAGE_SIZE pages to allocate
 */
void *page_alloc(int npages)
{
    if (npages <= 0 || _num_pages < npages) {
        return NULL;
    }

    /* Note we are searching the page descriptor bitmaps. */
    int found = 0;
    struct page *page_i = (struct page *)HEAP_START;
    for (int i = 0; i <= (_num_pages - npages); i++) {
        if (_is_page_free(page_i)) {
            found = 1;
            /* 
             * meet a free page, continue to check if the following
             * (npages - 1) pages are also unallocated.
             */
            struct page *page_j = page_i + 1;
            for (int j = i + 1; j < i + npages; j++) {
                if (!_is_page_free(page_j)) {
                    found = 0;
                    break;
                }
                page_j++;
            }
            /*
             * get a memory block which is good enough for us,
             * take housekeeping, then return the actual start
             * address of the first page of this memory block.
             */
            if (found) {
                struct page *page_k = page_i;
                for (int k = i; k < i + npages; k++) {
                    _set_page_flag(page_k, PAGE_TAKEN);
                    page_k++;
                }
                page_k--;
                _set_page_flag(page_k, PAGE_LAST);
                return (void *)(_alloc_start + i * PAGE_SIZE);
            }
        }
        page_i++;
    }
    return NULL;
}

/*
 * Free the memory block
 * - p: start address of the memory block
 */
void page_free(void *p)
{
    /* 
     * Assert (TBD) if p is invalid
     */
    if (!p || (uint32_t)p >= _alloc_end) {
        return;
    }
    /* get the first page descriptor of this memory block */
    struct page *page = (struct page *)HEAP_START;
    page += ((uint32_t)p - _alloc_start) / PAGE_SIZE;
    /* loop and clear all the page descriptors of the memory block */
    while (!_is_page_free(page)) {
        _clear_page(page);
        if (_is_page_last(page)) {
            break;
        }
        page++;
    }
}