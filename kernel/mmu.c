#include "../include/mmu.h"
#include "../include/printf.h"

#define PAGE_SIZE       4096
#define TABLE_ENTRIES   512

static unsigned long page_table[TABLE_ENTRIES] __attribute__((aligned(4096)));

void mmu_init() {
    for (int i = 0; i < TABLE_ENTRIES; i++) {
        unsigned long addr = (unsigned long)i * 0x40000000;
        page_table[i] = addr | (1 << 10) | (1 << 2) | (1 << 1) | 1;
    }

    asm volatile("msr ttbr0_el1, %0" :: "r"(page_table));

    unsigned long mair = 0xFF;
    asm volatile("msr mair_el1, %0" :: "r"(mair));

    unsigned long tcr = (16UL << 0) | (0UL << 14) | (2UL << 32);
    asm volatile("msr tcr_el1, %0" :: "r"(tcr));

    printf("[MMU] Page table initializat.\n");
}

void mmu_enable() {
    unsigned long sctlr;
    asm volatile("mrs %0, sctlr_el1" : "=r"(sctlr));
    sctlr |= 1;
    asm volatile("msr sctlr_el1, %0" :: "r"(sctlr));
    asm volatile("isb");
    printf("[MMU] MMU activat.\n");
}

void mmu_disable() {
    unsigned long sctlr;
    asm volatile("mrs %0, sctlr_el1" : "=r"(sctlr));
    sctlr &= ~1UL;
    asm volatile("msr sctlr_el1, %0" :: "r"(sctlr));
    asm volatile("isb");
}
