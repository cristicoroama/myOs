#include "../include/sched.h"
#include "../include/printf.h"
#include "../include/mem.h"

static Task   tasks[MAX_TASKS];
static int    task_count   = 0;
static int    current_task = 0;

void sched_init() {
    task_count   = 0;
    current_task = 0;
    for (int i = 0; i < MAX_TASKS; i++) {
        tasks[i].state = TASK_DEAD;
    }
    printf("[SCHED] Scheduler initializat. Max tasks: %d\n", MAX_TASKS);
}

int task_create(const char* name, void (*entry)()) {
    if (task_count >= MAX_TASKS) {
        printf("[SCHED] EROARE: Numarul maxim de task-uri atins!\n");
        return -1;
    }

    Task* t = &tasks[task_count];

    t->id    = task_count;
    t->name  = name;
    t->state = TASK_READY;

    for (int i = 0; i < TASK_STACK_SIZE; i++) {
        t->stack[i] = 0;
    }

    t->context.pc   = (unsigned long)entry;
    t->context.sp   = (unsigned long)(t->stack + TASK_STACK_SIZE);
    t->context.spsr = 0x3c5;

    printf("[SCHED] Task creat: id=%d name=%s entry=%x\n",
           t->id, t->name, t->context.pc);

    task_count++;
    return t->id;
}

void schedule() {
    if (task_count == 0) return;

    int next = current_task;

    for (int i = 1; i <= MAX_TASKS; i++) {
        int candidate = (current_task + i) % task_count;
        if (tasks[candidate].state == TASK_READY ||
            tasks[candidate].state == TASK_RUNNING) {
            next = candidate;
            break;
        }
    }

    if (next == current_task) return;

    tasks[current_task].state = TASK_READY;
    tasks[next].state         = TASK_RUNNING;
    current_task              = next;
}

void task_yield() {
    schedule();
}

void sched_stats() {
    printf("[SCHED] Task-uri active: %d\n", task_count);
    for (int i = 0; i < task_count; i++) {
        const char* state_str;
        if      (tasks[i].state == TASK_RUNNING) state_str = "RUNNING";
        else if (tasks[i].state == TASK_READY)   state_str = "READY";
        else                                       state_str = "DEAD";
        printf("  [%d] %s -> %s\n", tasks[i].id, tasks[i].name, state_str);
    }
}
