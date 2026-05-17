#include "../include/shell.h"
#include "../include/uart.h"
#include "../include/printf.h"
#include "../include/mem.h"
#include "../include/sched.h"

static char buf[SHELL_BUF_SIZE];
static int  buf_len = 0;

static int str_equals(const char* a, const char* b) {
    while (*a && *b) {
        if (*a != *b) return 0;
        a++; b++;
    }
    return *a == *b;
}

static int str_starts(const char* str, const char* prefix) {
    while (*prefix) {
        if (*str != *prefix) return 0;
        str++; prefix++;
    }
    return 1;
}

static void cmd_help() {
    printf("\nComenzi disponibile:\n");
    printf("  help      - afiseaza aceasta lista\n");
    printf("  clear     - curata ecranul\n");
    printf("  meminfo   - informatii despre memorie\n");
    printf("  tasks     - afiseaza task-urile active\n");
    printf("  echo      - afiseaza un mesaj (ex: echo salut)\n");
    printf("  reboot    - restarteaza kernelul\n");
    printf("  version   - versiunea OS-ului\n");
    printf("\n");
}

static void cmd_clear() {
    for (int i = 0; i < 40; i++) {
        printf("\n");
    }
}

static void cmd_meminfo() {
    mem_stats();
}

static void cmd_tasks() {
    sched_stats();
}

static void cmd_echo(const char* input) {
    const char* msg = input + 5;
    printf("%s\n", msg);
}

static void cmd_version() {
    printf("\n");
    printf("  MyOS v0.1\n");
    printf("  Platform : Raspberry Pi 3B+\n");
    printf("  Arch     : AArch64 (ARM 64-bit)\n");
    printf("  Build    : Bare Metal\n");
    printf("\n");
}

static void cmd_reboot() {
    printf("Reboot...\n");

    volatile unsigned int* PM_WDOG = (volatile unsigned int*)(0x3F100024);
    volatile unsigned int* PM_RSTC = (volatile unsigned int*)(0x3F10001C);
    *PM_WDOG = 0x5a000001;
    *PM_RSTC = 0x5a000020;

    while(1) {}
}

static void cmd_unknown(const char* cmd) {
    printf("Comanda necunoscuta: '%s'\n", cmd);
    printf("Scrie 'help' pentru lista de comenzi.\n");
}

static void execute(const char* cmd) {
    if (cmd[0] == '\0') return;

    if (str_equals(cmd, "help"))         cmd_help();
    else if (str_equals(cmd, "clear"))   cmd_clear();
    else if (str_equals(cmd, "meminfo")) cmd_meminfo();
    else if (str_equals(cmd, "tasks"))   cmd_tasks();
    else if (str_equals(cmd, "version")) cmd_version();
    else if (str_equals(cmd, "reboot"))  cmd_reboot();
    else if (str_starts(cmd, "echo "))   cmd_echo(cmd);
    else                                  cmd_unknown(cmd);
}

void shell_init() {
    buf_len = 0;
    for (int i = 0; i < SHELL_BUF_SIZE; i++) buf[i] = 0;
    printf("\nMyOS Shell - scrie 'help' pentru comenzi\n\n");
    printf("> ");
}

void shell_run() {
    char c = uart_getc();

    if (c == '\r' || c == '\n') {
        printf("\n");
        buf[buf_len] = '\0';
        execute(buf);
        buf_len = 0;
        for (int i = 0; i < SHELL_BUF_SIZE; i++) buf[i] = 0;
        printf("> ");
    }
    else if (c == 127 || c == '\b') {
        if (buf_len > 0) {
            buf_len--;
            buf[buf_len] = '\0';
            uart_putc('\b');
            uart_putc(' ');
            uart_putc('\b');
        }
    }
    else if (buf_len < SHELL_BUF_SIZE - 1) {
        buf[buf_len++] = c;
        uart_putc(c);
    }
}
