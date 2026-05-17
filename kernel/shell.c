#include "../include/shell.h"
#include "../include/uart.h"
#include "../include/printf.h"
#include "../include/mem.h"
#include "../include/sched.h"
#include "../include/timer.h"
#include "../include/gpio.h"
#include "../include/string.h"
#include "../include/fs.h"
#include "../include/process.h"

static char buf[SHELL_BUF_SIZE];
static int  buf_len = 0;

static void cmd_help() {
    printf("\nComenzi disponibile:\n");
    printf("  help               - afiseaza aceasta lista\n");
    printf("  clear              - curata ecranul\n");
    printf("  version            - versiunea OS\n");
    printf("  uptime             - timp de functionare\n");
    printf("  meminfo            - informatii memorie\n");
    printf("  tasks              - task-uri scheduler\n");
    printf("  ps                 - lista procese\n");
    printf("  kill <pid>         - termina un proces\n");
    printf("  ls                 - lista fisiere\n");
    printf("  create <nume>      - creeaza fisier\n");
    printf("  write <nume> <txt> - scrie in fisier\n");
    printf("  read <nume>        - citeste fisier\n");
    printf("  rm <nume>          - sterge fisier\n");
    printf("  echo <text>        - afiseaza text\n");
    printf("  led <on/off>       - controleaza LED GPIO 17\n");
    printf("  malloc <n>         - aloca n bytes\n");
    printf("  reboot             - restart\n");
    printf("\n");
}

static void cmd_clear() {
    for (int i = 0; i < 50; i++) printf("\n");
}

static void cmd_version() {
    printf("\n  MyOS v0.1 | RPi 3B+ | AArch64\n\n");
}

static void cmd_uptime() {
    unsigned long ms = get_uptime_ms();
    unsigned long s  = ms / 1000;
    unsigned long m  = s / 60;
    unsigned long h  = m / 60;
    printf("Uptime: %u ore %u min %u sec\n", h, m % 60, s % 60);
}

static void cmd_led(const char* input) {
    const char* arg = input + 4;
    gpio_init(17, 1);
    if (strcmp(arg, "on") == 0)       { gpio_set(17);   printf("LED ON\n");  }
    else if (strcmp(arg, "off") == 0) { gpio_clear(17); printf("LED OFF\n"); }
    else printf("Foloseste: led on / led off\n");
}

static void cmd_malloc(const char* input) {
    const char* arg = input + 7;
    unsigned long size = 0;
    while (*arg >= '0' && *arg <= '9') { size = size * 10 + (*arg - '0'); arg++; }
    if (!size) { printf("Foloseste: malloc <numar>\n"); return; }
    void* ptr = malloc(size);
    if (ptr) printf("malloc(%u) -> %x\n", size, ptr);
    else     printf("EROARE: out of memory\n");
}

static void cmd_kill(const char* input) {
    const char* arg = input + 5;
    int pid = 0;
    while (*arg >= '0' && *arg <= '9') { pid = pid * 10 + (*arg - '0'); arg++; }
    proc_kill(pid);
}

static void cmd_create(const char* input) {
    const char* name = input + 7;
    int r = fs_create(name);
    if      (r == FS_OK)          printf("Fisier '%s' creat.\n", name);
    else if (r == FS_ERR_EXISTS)  printf("EROARE: fisierul exista deja.\n");
    else if (r == FS_ERR_FULL)    printf("EROARE: filesystem plin.\n");
}

static void cmd_write(const char* input) {
    const char* p = input + 6;
    char name[MAX_FILENAME];
    int i = 0;
    while (*p && *p != ' ' && i < MAX_FILENAME - 1) { name[i++] = *p++; }
    name[i] = '\0';
    if (*p == ' ') p++;
    if (!fs_exists(name)) fs_create(name);
    fs_write(name, p, strlen(p));
    printf("Scris in '%s'.\n", name);
}

static void cmd_read(const char* input) {
    const char* name = input + 5;
    char buf2[MAX_FILESIZE];
    int r = fs_read(name, buf2, MAX_FILESIZE - 1);
    if (r < 0) printf("EROARE: fisierul '%s' nu exista.\n", name);
    else { buf2[r] = '\0'; printf("%s\n", buf2); }
}

static void cmd_rm(const char* input) {
    const char* name = input + 3;
    int r = fs_delete(name);
    if (r == FS_OK)            printf("Fisier '%s' sters.\n", name);
    else if (r == FS_ERR_NOTFOUND) printf("EROARE: fisierul nu exista.\n");
}

static void execute(const char* cmd) {
    if (!cmd[0]) return;

    if      (strcmp(cmd, "help")     == 0)  cmd_help();
    else if (strcmp(cmd, "clear")    == 0)  cmd_clear();
    else if (strcmp(cmd, "version")  == 0)  cmd_version();
    else if (strcmp(cmd, "uptime")   == 0)  cmd_uptime();
    else if (strcmp(cmd, "meminfo")  == 0)  mem_stats();
    else if (strcmp(cmd, "tasks")    == 0)  sched_stats();
    else if (strcmp(cmd, "ps")       == 0)  proc_list();
    else if (strcmp(cmd, "ls")       == 0)  fs_list();
    else if (strcmp(cmd, "reboot")   == 0) {
        volatile unsigned int* PM_WDOG = (volatile unsigned int*)(0x3F100024);
        volatile unsigned int* PM_RSTC = (volatile unsigned int*)(0x3F10001C);
        *PM_WDOG = 0x5a000001;
        *PM_RSTC = 0x5a000020;
        while(1) {}
    }
    else if (strncmp(cmd, "echo ",   5) == 0) printf("%s\n", cmd + 5);
    else if (strncmp(cmd, "led ",    4) == 0) cmd_led(cmd);
    else if (strncmp(cmd, "malloc ", 7) == 0) cmd_malloc(cmd);
    else if (strncmp(cmd, "kill ",   5) == 0) cmd_kill(cmd);
    else if (strncmp(cmd, "create ", 7) == 0) cmd_create(cmd);
    else if (strncmp(cmd, "write ",  6) == 0) cmd_write(cmd);
    else if (strncmp(cmd, "read ",   5) == 0) cmd_read(cmd);
    else if (strncmp(cmd, "rm ",     3) == 0) cmd_rm(cmd);
    else {
        printf("Comanda necunoscuta: '%s'\n", cmd);
        printf("Scrie 'help'.\n");
    }
}

void shell_init() {
    buf_len = 0;
    memset(buf, 0, SHELL_BUF_SIZE);
    printf("\nMyOS Shell - scrie 'help'\n\n");
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
            uart_putc('\b'); uart_putc(' '); uart_putc('\b');
        }
    } else if (buf_len < SHELL_BUF_SIZE - 1) {
        buf[buf_len++] = c;
        uart_putc(c);
    }
}
