#include "../include/mem.h"
#include "../include/printf.h"

#define HEAP_START  0x100000
#define HEAP_END    0x3AF00000
#define ALIGN       8

static unsigned long heap_ptr = HEAP_START;
static unsigned long alloc_count = 0;
static unsigned long total_allocated = 0;

void mem_init() {
    heap_ptr = HEAP_START;
    alloc_count = 0;
    total_allocated = 0;
}

void* malloc(unsigned long size) {
    if (size == 0) return (void*)0;

    if (size % ALIGN != 0) {
        size = size + (ALIGN - (size % ALIGN));
    }

    if (heap_ptr + size > HEAP_END) {
        printf("[MEM] EROARE: Out of memory!\n");
        return (void*)0;
    }

    void* ptr = (void*)heap_ptr;
    heap_ptr += size;
    alloc_count++;
    total_allocated += size;

    return ptr;
}

void free(void* ptr) {
    (void)ptr;
}

void mem_stats() {
    printf("[MEM] Heap start:     %x\n", HEAP_START);
    printf("[MEM] Heap end:       %x\n", HEAP_END);
    printf("[MEM] Heap ptr:       %x\n", heap_ptr);
    printf("[MEM] Total alocari:  %u\n", alloc_count);
    printf("[MEM] Total alocat:   %u bytes\n", total_allocated);
    printf("[MEM] Liber:          %u bytes\n", HEAP_END - heap_ptr);
}
