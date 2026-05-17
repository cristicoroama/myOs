#include "../include/shell.h"
#include "../include/uart.h"
#include "../include/printf.h"
#include "../include/mem.h"
#include "../include/sched.h"
#include "../include/timer.h"
#include "../include/gpio.h"
#include "../include/string.h"

static char buf[SHELL_BUF_SIZE];
static int  buf_len = 0;

static void cmd_help() {
    printf("\nComenzi disponibile:\n");
    printf("  help          - afiseaza aceasta lista\n");
    printf("  clear         - curata ecranul\n");
    printf("  meminfo       - informatii despre memorie\n");
    printf("  tasks         - afiseaza task-urile active\n");
    printf("  echo <text>   - afiseaza un mesaj\n");
    printf("  uptime        - timp de functionare\n");
    printf("  led <on/off>  - controleaza LED-ul de pe GPIO 17\n");
    printf("  malloc <n>    - aloca n bytes si afiseaza adresa\n");
    printf("  reboot        - restarteaza kernelul\n");
    printf("  version       - versiunea OS-ului\n");
    printf("\n");
}

static void cmd_clear() {
    for (int i = 0; i < 40; i++) printf("\n");
}

static void cmd_meminfo() {
    mem_stats();
}

static void cmd_tasks() {
    sched_stats();
}

static void cmd_echo(const char* input) {
    printf("%s\n", input + 5);
}

static void cmd_uptime() {
    unsigned long ms = get_uptime_ms();
    unsigned long s  = ms / 1000;
    unsigned long m  = s / 60;
    unsigned long h  = m / 60;
    printf("Uptime: %u ore, %u minute, %u secunde\n", h, m % 60, s % 60);
}

static void cmd_led(const char* input) {
    const char* arg = input + 4;
    gpio_init(17, 1);
    if (strcmp(arg, "on") == 0) {
        gpio_set(17);
        printf("LED GPIO 17: ON\n");
    } else if (strcmp(arg, "off") == 0) {
        gpio_clear(17);
        printf("LED GPIO 17: OFF\n");
    } else {
        printf("Foloseste: led on / led off\n");
    }
}

static void cmd_malloc(const char* input) {
    const char* arg = input + 7;
    unsigned long size = 0;
    while (*arg >= '0' && *arg <= '9') {
        size = size * 10 + (*arg - '0');
        arg++;
    }
    if (size == 0) {
        printf("Foloseste: malloc <numar>\n");
        return;
    }
    void* ptr = malloc(size);
    if (ptr) printf("malloc(%u) -> %x\n", size, ptr);
    else     printf("malloc(%u) -> EROARE: out of memory\n", size);
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

static void execute(const char* cmd) {
    if (cmd[0] == '\0') return;

    if      (strcmp(cmd, "help")    == 0)      cmd_help();
    else if (strcmp(cmd, "clear")   == 0)      cmd_clear();
    else if (strcmp(cmd, "meminfo") == 0)      cmd_meminfo();
    else if (strcmp(cmd, "tasks")   == 0)      cmd_tasks();
    else if (strcmp(cmd, "uptime")  == 0)      cmd_uptime();
    else if (strcmp(cmd, "version") == 0)      cmd_version();
    else if (strcmp(cmd, "reboot")  == 0)      cmd_reboot();
    else if (strncmp(cmd, "echo ",   5) == 0)  cmd_echo(cmd);
    else if (strncmp(cmd, "led ",    4) == 0)  cmd_led(cmd);
    else if (strncmp(cmd, "malloc ", 7) == 0)  cmd_malloc(cmd);
    else {
        printf("Comanda necunoscuta: '%s'\n", cmd);
        printf("Scrie 'help' pentru lista de comenzi.\n");
    }
}

void shell_init() {
    buf_len = 0;
    memset(buf, 0, SHELL_BUF_SIZE);
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
        memset(buf, 0, SHELL_BUF_SIZE);
        printf("> ");
    } else if (c == 127 || c == '\b') {
        if (buf_len > 0) {
            buf_len--;
            buf[buf_len] = '\0';
            uart_putc('\b');
            uart_putc(' ');
            uart_putc('\b');
        }
    } else if (buf_len < SHELL_BUF_SIZE - 1) {
        buf[buf_len++] = c;
        uart_putc(c);
    }
}
