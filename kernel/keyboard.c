#include "../include/keyboard.h"
#include "../include/uart.h"

void keyboard_init() {
}

char keyboard_getc() {
    char c = uart_getc();

    if (c == 0x1B) {
        char c2 = uart_getc();
        if (c2 == '[') {
            char c3 = uart_getc();
            return c3;
        }
    }

    return c;
}

int keyboard_available() {
    return 1;
}
