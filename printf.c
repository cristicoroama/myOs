#include "../include/uart.h"
#include "../include/printf.h"

static void print_uint(unsigned long n, int base) {
    char digits[] = "0123456789abcdef";
    char buf[64];
    int i = 0;

    if (n == 0) {
        uart_putc('0');
        return;
    }

    while (n > 0) {
        buf[i++] = digits[n % base];
        n /= base;
    }

    while (i--) {
        uart_putc(buf[i]);
    }
}

static void print_int(long n) {
    if (n < 0) {
        uart_putc('-');
        n = -n;
    }
    print_uint(n, 10);
}

void printf(const char* fmt, ...) {
    __builtin_va_list args;
    __builtin_va_start(args, fmt);

    while (*fmt) {
        if (*fmt == '%') {
            fmt++;
            switch (*fmt) {
                case 'c':
                    uart_putc((char)__builtin_va_arg(args, int));
                    break;
                case 's':
                    uart_puts(__builtin_va_arg(args, const char*));
                    break;
                case 'd':
                    print_int(__builtin_va_arg(args, long));
                    break;
                case 'u':
                    print_uint(__builtin_va_arg(args, unsigned long), 10);
                    break;
                case 'x':
                    uart_puts("0x");
                    print_uint(__builtin_va_arg(args, unsigned long), 16);
                    break;
                case '%':
                    uart_putc('%');
                    break;
            }
        } else {
            if (*fmt == '\n') uart_putc('\r');
            uart_putc(*fmt);
        }
        fmt++;
    }

    __builtin_va_end(args);
}
