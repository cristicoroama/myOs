#ifndef GPIO_H
#define GPIO_H

#define GPIO_INPUT  0
#define GPIO_OUTPUT 1

void gpio_init(int pin, int mode);
void gpio_set(int pin);
void gpio_clear(int pin);
int  gpio_read(int pin);
void gpio_toggle(int pin);

#endif
