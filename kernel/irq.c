#include "../include/irq.h"
#include "../include/printf.h"

#define MMIO_BASE           0x3F000000

#define IRQ_BASE            (MMIO_BASE + 0xB000)
#define IRQ_BASIC_PENDING   ((volatile unsigned int*)(IRQ_BASE + 0x200))
#define IRQ_PENDING1        ((volatile unsigned int*)(IRQ_BASE + 0x204))
#define IRQ_PENDING2        ((volatile unsigned int*)(IRQ_BASE + 0x208))
#define IRQ_ENABLE1         ((volatile unsigned int*)(IRQ_BASE + 0x210))
#define IRQ_ENABLE2         ((volatile unsigned int*)(IRQ_BASE + 0x214))
#define IRQ_ENABLE_BASIC    ((volatile unsigned int*)(IRQ_BASE + 0x218))

#define TIMER_BASE          (MMIO_BASE + 0x3000)
#define TIMER_CS            ((volatile unsigned int*)(TIMER_BASE + 0x00))
#define TIMER_CLO           ((volatile unsigned int*)(TIMER_BASE + 0x04))
#define TIMER_C1            ((volatile unsigned int*)(TIMER_BASE + 0x10))
#define TIMER_CS_M1         (1 << 1)

#define TIMER_INTERVAL      1000000

static unsigned int timer_count = 0;

void irq_enable() {
    asm volatile("msr daifclr, #2");
}

void irq_disable() {
    asm volatile("msr daifset, #2");
}

void timer_init() {
    *TIMER_C1 = *TIMER_CLO + TIMER_INTERVAL;
    *IRQ_ENABLE1 = (1 << 1);
}

void handle_sync() {
    printf("[SYNC] Synchronous exception!\n");
    while(1) {}
}

void handle_irq() {
    if (*IRQ_PENDING1 & (1 << 1)) {
        *TIMER_CS = TIMER_CS_M1;
        *TIMER_C1 = *TIMER_CLO + TIMER_INTERVAL;
        timer_count++;
        printf("[TIMER] Tick %u\n", timer_count);
    }
}

void handle_fiq() {
    printf("[FIQ] Fast interrupt!\n");
}

void handle_error() {
    printf("[ERROR] System error!\n");
    while(1) {}
}
