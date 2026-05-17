#include "../include/gpio.h"

#define MMIO_BASE   0x3F000000
#define GPIO_BASE   (MMIO_BASE + 0x200000)

#define GPFSEL(n)   ((volatile unsigned int*)(GPIO_BASE + (n) * 4))
#define GPSET0      ((volatile unsigned int*)(GPIO_BASE + 0x1C))
#define GPCLR0      ((volatile unsigned int*)(GPIO_BASE + 0x28))
#define GPLEV0      ((volatile unsigned int*)(GPIO_BASE + 0x34))

void gpio_init(int pin, int mode) {
    int reg    = pin / 10;
    int shift  = (pin % 10) * 3;
    unsigned int val = *GPFSEL(reg);
    val &= ~(7 << shift);
    val |= (mode & 7) << shift;
    *GPFSEL(reg) = val;
}

void gpio_set(int pin) {
    *GPSET0 = (1 << pin);
}

void gpio_clear(int pin) {
    *GPCLR0 = (1 << pin);
}

int gpio_read(int pin) {
    return (*GPLEV0 >> pin) & 1;
}

void gpio_toggle(int pin) {
    if (gpio_read(pin)) gpio_clear(pin);
    else                gpio_set(pin);
}
