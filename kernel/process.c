#include "../include/process.h"
#include "../include/printf.h"
#include "../include/string.h"

static Process processes[MAX_PROCESSES];
static int     proc_count  = 0;
static int     current_pid = 0;

void proc_init() {
    proc_count  = 0;
    current_pid = 0;
    for (int i = 0; i < MAX_PROCESSES; i++) {
        processes[i].state     = PROC_DEAD;
        processes[i].pid       = i;
        processes[i].exit_code = 0;
    }
    printf("[PROC] Process manager initializat. Max: %d\n", MAX_PROCESSES);
}

int proc_create(const char* name, void (*entry)()) {
    if (proc_count >= MAX_PROCESSES) {
        printf("[PROC] EROARE: prea multe procese!\n");
        return -1;
    }

    Process* p = &processes[proc_count];
    p->name    = name;
    p->state   = PROC_READY;
    p->pc      = (unsigned long)entry;
    p->sp      = (unsigned long)(p->stack + (PROC_STACK_SIZE / 8) - 1);

    printf("[PROC] Proces creat: pid=%d name=%s\n", p->pid, p->name);
    proc_count++;
    return p->pid;
}

void proc_exit(int code) {
    processes[current_pid].state     = PROC_DEAD;
    processes[current_pid].exit_code = code;
    printf("[PROC] pid=%d exit(%d)\n", current_pid, code);
}

void proc_kill(int pid) {
    if (pid < 0 || pid >= proc_count) {
        printf("[PROC] EROARE: pid=%d nu exista\n", pid);
        return;
    }
    processes[pid].state = PROC_DEAD;
    printf("[PROC] pid=%d terminat\n", pid);
}

int proc_getpid() {
    return current_pid;
}

void proc_list() {
    printf("\nProcese:\n");
    for (int i = 0; i < proc_count; i++) {
        const char* st;
        if      (processes[i].state == PROC_RUNNING) st = "RUNNING";
        else if (processes[i].state == PROC_READY)   st = "READY";
        else if (processes[i].state == PROC_BLOCKED)  st = "BLOCKED";
        else                                           st = "DEAD";
        printf("  pid=%d name=%s state=%s\n", processes[i].pid, processes[i].name, st);
    }
    printf("\n");
}
