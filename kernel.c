#include "../include/uart.h"
#include "../include/printf.h"

void kernel_main() {
    uart_init();

    printf("=============================\n");
    printf("       MyOS - RPi 3B+        \n");
    printf("=============================\n");
    printf("Kernel pornit cu succes!\n");
    printf("Core activ: %d\n", 0);
    printf("Kernel incarcat la: %x\n", 0x80000);
    printf("\n> ");

    while (1) {
        char c = uart_getc();
        uart_putc(c);
        if (c == '\r') {
            printf("\n> ");
        }
    }
}
