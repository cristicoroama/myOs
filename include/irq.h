#ifndef IRQ_H
#define IRQ_H

void irq_init_vectors();
void irq_enable();
void irq_disable();
void timer_init();

void handle_sync();
void handle_irq();
void handle_fiq();
void handle_error();

#endif
