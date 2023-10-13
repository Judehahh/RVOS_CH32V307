#include "os.h"

// Memory allocator by Kernighan and Ritchie,
// The C programming Language, 2nd ed. Section 8.7.

typedef long Align;     /* use the long type for alignment */

union header {
    struct {
        union header *next;
        uint32_t size;
    } s;
    Align alignment;    /* never used, only for alignment */
};

typedef union header Header;

static Header base;
static Header *freep = NULL;

void free(void *ap)
{
    Header *bp, *p;

    bp = (Header *)ap - 1;

    /* find the correct place to insert */
    for (p = freep; !(bp > p && bp < p->s.next); p = p->s.next)
        if (p >= p->s.next && (bp > p || bp < p->s.next))
            break;

    /* merge with the next free block */
    if (bp + bp->s.size == p->s.next) {
        bp->s.size += p->s.next->s.size;
        bp->s.next = p->s.next->s.next;
    } else
        bp->s.next = p->s.next;

    /* merge with the previous free block */
    if (p + p->s.size == bp) {
        p->s.size += bp->s.size;
        p->s.next = bp->s.next;
    } else
        p->s.next = bp;
        
    freep = p;
}

void *malloc(uint32_t nbytes)
{
    Header *prevp = freep;  /* start where the last free block was found */
    uint32_t nunints;

    /*
     * - add sizeof(Header) to round up.
     * - sub one to avoid allocating an extra Header space when 
     *   nbytes is exactly an integer multiple of sizeof(Header).
     * - add one to add a Header for managing stored information.
     */
    nunints = (nbytes + sizeof(Header) - 1) / sizeof(Header) + 1;

    if (prevp == NULL) {  /* first time malloc */
        base.s.next = freep = prevp = &base;
        base.s.size = 0;

        /* alloc 2 pages for malloc */
        void *p = page_alloc(2);
        if (p != NULL) {
            Header *up = (Header *)p;
            up->s.size = 2 * 4096 / sizeof(Header);
            free((void *)(up + 1));
        }
    }

    for (Header *p = prevp->s.next; ; prevp = p, p = p->s.next) {
        if (p->s.size >= nunints) {     /* have enough space */
            if (p->s.size == nunints)
                prevp->s.next = p->s.next;
            else {
                p->s.size -= nunints;
                p += p->s.size;
                p->s.size = nunints;
            }
            freep = prevp;
            return (void *)(p + 1);     /* skip the head */
        }
        if (p == freep) {
            printf("[ERROR] malloc() no enough blocks.\r\n");
            while (1) {}
        }
    }
}

void print_memory() {
    Header *p = freep;

    if (p == NULL) {
        printf("No memory has been allocated.\n");
        return;
    }

    printf("Memory blocks:\n");
    do {
        printf("Location: %p, size: %d\n", (void *)p, p->s.size * sizeof(Header));
        p = p->s.next;
    } while (p != freep);
}

