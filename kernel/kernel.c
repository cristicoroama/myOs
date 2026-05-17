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

    printf("\n");
    printf("  ███╗   ███╗██╗   ██╗ ██████╗ ███████╗\n");
    printf("  ████╗ ████║╚██╗ ██╔╝██╔═══██╗██╔════╝\n");
    printf("  ██╔████╔██║ ╚████╔╝ ██║   ██║███████╗\n");
    printf("  ██║╚██╔╝██║  ╚██╔╝  ██║   ██║╚════██║\n");
    printf("  ██║ ╚═╝ ██║   ██║   ╚██████╔╝███████║\n");
    printf("  ╚═╝     ╚═╝   ╚═╝    ╚═════╝ ╚══════╝\n");
    printf("\n");
    printf("  Raspberry Pi 3B+ | AArch64 | v0.1\n");
    printf("  =====================================\n\n");

    irq_init_vectors();  printf("[OK] Vector table\n");
    timer_init();        printf("[OK] Timer\n");
    irq_enable();        printf("[OK] Interrupts\n");
    mem_init();          printf("[OK] Memory manager\n");
    mmu_init();          printf("[OK] MMU\n");
    sched_init();        printf("[OK] Scheduler\n");
    proc_init();         printf("[OK] Process manager\n");
    fs_init();           printf("[OK] Filesystem\n");
    display_init();      printf("[OK] Display\n");

    gpio_init(17, 1);
    gpio_set(17);
    printf("[OK] GPIO - LED on pin 17\n");

    shell_init();

    while (1) {
        shell_run();
    }
}
