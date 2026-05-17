#include "../include/syscall.h"
#include "../include/uart.h"
#include "../include/printf.h"
#include "../include/mem.h"
#include "../include/timer.h"

long syscall(long num, long a0, long a1, long a2) {
    switch (num) {
        case SYS_WRITE:
            uart_puts((const char*)a0);
            return 0;

        case SYS_READ: {
            char* buf = (char*)a0;
            int   len = (int)a1;
            for (int i = 0; i < len; i++) {
                buf[i] = uart_getc();
                if (buf[i] == '\r') { buf[i] = '\0'; break; }
            }
            return 0;
        }

        case SYS_MALLOC:
            return (long)malloc((unsigned long)a0);

        case SYS_FREE:
            free((void*)a0);
            return 0;

        case SYS_EXIT:
            printf("[SYSCALL] exit(%d)\n", (int)a0);
            while(1) {}
            return 0;

        case SYS_GETPID:
            return 0;

        case SYS_UPTIME:
            return (long)get_uptime_ms();

        default:
            printf("[SYSCALL] Necunoscut: %d\n", (int)num);
            return -1;
    }
}
