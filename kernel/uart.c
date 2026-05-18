#include "../include/uart.h"

#define MMIO_BASE       0x3F000000UL

#define GPIO_BASE       (MMIO_BASE + 0x200000)
#define GPFSEL1         ((volatile unsigned int*)(GPIO_BASE + 0x04))
#define GPPUD           ((volatile unsigned int*)(GPIO_BASE + 0x94))
#define GPPUDCLK0       ((volatile unsigned int*)(GPIO_BASE + 0x98))

#define UART0_BASE      (MMIO_BASE + 0x201000)
#define UART0_DR        ((volatile unsigned int*)(UART0_BASE + 0x00))
#define UART0_FR        ((volatile unsigned int*)(UART0_BASE + 0x18))
#define UART0_IBRD      ((volatile unsigned int*)(UART0_BASE + 0x24))
#define UART0_FBRD      ((volatile unsigned int*)(UART0_BASE + 0x28))
#define UART0_LCRH      ((volatile unsigned int*)(UART0_BASE + 0x2C))
#define UART0_CR        ((volatile unsigned int*)(UART0_BASE + 0x30))
#define UART0_IMSC      ((volatile unsigned int*)(UART0_BASE + 0x38))

static void delay(int count) {
    while (count--) asm volatile("nop");
}

void uart_init() {
    *UART0_CR = 0;

    unsigned int ra = *GPFSEL1;
    ra &= ~(7 << 12);
    ra |=  (4 << 12);
    ra &= ~(7 << 15);
    ra |=  (4 << 15);
    *GPFSEL1 = ra;

    *GPPUD = 0;
    delay(150);
    *GPPUDCLK0 = (1 << 14) | (1 << 15);
    delay(150);
    *GPPUDCLK0 = 0;

    *UART0_IBRD = 26;
    *UART0_FBRD = 3;

    *UART0_LCRH = (1 << 4) | (1 << 5) | (1 << 6);
    *UART0_IMSC = 0;
    *UART0_CR = (1 << 0) | (1 << 8) | (1 << 9);
}

void uart_putc(char c) {
    while (*UART0_FR & (1 << 5)) {}
    *UART0_DR = c;
}

char uart_getc() {
    while (*UART0_FR & (1 << 4)) {}
    return (char)(*UART0_DR & 0xFF);
}

void uart_puts(const char* str) {
    while (*str) {
        if (*str == '\n') uart_putc('\r');
        uart_putc(*str++);
    }
}
