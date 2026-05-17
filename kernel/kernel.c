#include "../include/uart.h"
#include "../include/printf.h"
#include "../include/irq.h"
#include "../include/mem.h"
#include "../include/sched.h"

void task_a() {
    while (1) {
        printf("[Task A] Rulez!\n");
        task_yield();
    }
}

void task_b() {
    while (1) {
        printf("[Task B] Rulez!\n");
        task_yield();
    }
}

void task_c() {
    while (1) {
        printf("[Task C] Rulez!\n");
        task_yield();
    }
}

void kernel_main() {
    uart_init();

    printf("=============================\n");
    printf("       MyOS - RPi 3B+        \n");
    printf("=============================\n");
    printf("Kernel pornit cu succes!\n");
    printf("Core activ:         %d\n", 0);
    printf("Kernel incarcat la: %x\n", 0x80000);
    printf("\n");

    irq_init_vectors();
    printf("Vector table initializat.\n");

    timer_init();
    printf("Timer initializat.\n");

    irq_enable();
    printf("Interrupts activate.\n");

    mem_init();
    printf("Memory manager initializat.\n");

    sched_init();

    task_create("TaskA", task_a);
    task_create("TaskB", task_b);
    task_create("TaskC", task_c);

    sched_stats();

    printf("\nStarting scheduler...\n\n");

    while (1) {
        schedule();
    }
}
