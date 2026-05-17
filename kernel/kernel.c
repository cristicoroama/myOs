#include "../include/uart.h"
#include "../include/printf.h"
#include "../include/irq.h"
#include "../include/mem.h"

void kernel_main() {
    uart_init();

    printf("=============================\n");
    printf("       MyOS - RPi 3B+        \n");
    printf("=============================\n");
    printf("Kernel pornit cu succes!\n");
    printf("Core activ: %d\n", 0);
    printf("Kernel incarcat la: %x\n", 0x80000);

    irq_init_vectors();
    printf("Vector table initializat.\n");

    timer_init();
    printf("Timer initializat.\n");

    irq_enable();
    printf("Interrupts activate.\n");

    mem_init();
    printf("Memory manager initializat.\n");

    mem_stats();

    void* a = malloc(64);
    void* b = malloc(256);
    printf("\nTest malloc:\n");
    printf("  malloc(64)  -> %x\n", a);
    printf("  malloc(256) -> %x\n", b);

    mem_stats();

    printf("\n> ");

    while (1) {
        char c = uart_getc();
        uart_putc(c);
        if (c == '\r') {
            printf("\n> ");
        }
    }
}
