#include "../include/timer.h"

#define MMIO_BASE   0x3F000000
#define TIMER_BASE  (MMIO_BASE + 0x3000)
#define TIMER_CLO   ((volatile unsigned int*)(TIMER_BASE + 0x04))

static unsigned long boot_ticks = 0;

unsigned long get_ticks() {
    return *TIMER_CLO;
}

void delay_us(unsigned int us) {
    unsigned long start = get_ticks();
    while (get_ticks() - start < us);
}

void delay_ms(unsigned int ms) {
    delay_us(ms * 1000);
}

unsigned long get_uptime_ms() {
    return get_ticks() / 1000;
}
