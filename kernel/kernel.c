#include "../include/uart.h"
#include "../include/printf.h"
#include "../include/irq.h"
#include "../include/mem.h"
#include "../include/sched.h"
#include "../include/shell.h"
#include "../include/mmu.h"
#include "../include/timer.h"
#include "../include/gpio.h"
#include "../include/fs.h"
#include "../include/process.h"
#include "../include/display.h"

void kernel_main() {
    uart_init();

    display_init();
    printf_enable_display();
    display_set_color(COLOR_GREEN, COLOR_BLACK);

    printf("\n");
    printf("  ===================================\n");
    printf("       MyOS v0.1 - RPi 3B+\n");
    printf("       AArch64 Bare Metal\n");
    printf("  ===================================\n\n");

    display_set_color(COLOR_WHITE, COLOR_BLACK);

    irq_init_vectors();  printf("[OK] Vector table\n");
    timer_init();        printf("[OK] Timer\n");
    irq_enable();        printf("[OK] Interrupts\n");
    mem_init();          printf("[OK] Memory manager\n");
    sched_init();        printf("[OK] Scheduler\n");
    proc_init();         printf("[OK] Process manager\n");
    fs_init();           printf("[OK] Filesystem\n");

    gpio_init(17, 1);
    gpio_set(17);
    printf("[OK] GPIO - LED ON\n");

    shell_init();

    while (1) {
        shell_run();
    }
}
