#include "../include/uart.h"

void kernel_main() {
    uart_init();

    uart_puts("=============================\n");
    uart_puts("       MyOS - RPi 3B+        \n");
    uart_puts("=============================\n");
    uart_puts("\nKernel pornit cu succes!\n");
    uart_puts("UART functional.\n\n");
    uart_puts("> ");

    while (1) {
        char c = uart_getc();
        uart_putc(c);

        if (c == '\r') {
            uart_puts("\n> ");
        }
    }
}
