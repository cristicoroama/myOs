#include "../include/uart.h"
#include "../include/printf.h"
#include "../include/display.h"

static int display_enabled = 0;

void printf_enable_display() {
    display_enabled = 1;
}

static void out_char(char c) {
    uart_putc(c);
    if (display_enabled) display_putc(c);
}

static void out_str(const char* s) {
    while (*s) {
        if (*s == '\n') {
            uart_putc('\r');
            if (display_enabled) display_putc('\r');
        }
        out_char(*s++);
    }
}

static void print_uint(unsigned long n, int base) {
    char digits[] = "0123456789abcdef";
    char buf[64];
    int i = 0;
    if (n == 0) { out_char('0'); return; }
    while (n > 0) {
        buf[i++] = digits[n % base];
        n /= base;
    }
    while (i--) out_char(buf[i]);
}

static void print_int(long n) {
    if (n < 0) { out_char('-'); n = -n; }
    print_uint(n, 10);
}

void printf(const char* fmt, ...) {
    __builtin_va_list args;
    __builtin_va_start(args, fmt);
    while (*fmt) {
        if (*fmt == '%') {
            fmt++;
            switch (*fmt) {
                case 'c': out_char((char)__builtin_va_arg(args, int)); break;
                case 's': out_str(__builtin_va_arg(args, const char*)); break;
                case 'd': print_int(__builtin_va_arg(args, long)); break;
                case 'u': print_uint(__builtin_va_arg(args, unsigned long), 10); break;
                case 'x':
                    out_str("0x");
                    print_uint(__builtin_va_arg(args, unsigned long), 16);
                    break;
                case '%': out_char('%'); break;
            }
        } else {
            if (*fmt == '\n') {
                uart_putc('\r');
                if (display_enabled) display_putc('\r');
            }
            out_char(*fmt);
        }
        fmt++;
    }
    __builtin_va_end(args);
}
