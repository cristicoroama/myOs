#ifndef TIMER_H
#define TIMER_H

void     delay_ms(unsigned int ms);
void     delay_us(unsigned int us);
unsigned long get_ticks();
unsigned long get_uptime_ms();

#endif
