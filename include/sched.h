#ifndef SCHED_H
#define SCHED_H

#define MAX_TASKS       8
#define TASK_STACK_SIZE 4096

#define TASK_RUNNING    0
#define TASK_READY      1
#define TASK_DEAD       2

typedef struct {
    unsigned long x0, x1, x2, x3, x4, x5, x6, x7;
    unsigned long x8, x9, x10, x11, x12, x13, x14, x15;
    unsigned long x16, x17, x18, x19, x20, x21, x22, x23;
    unsigned long x24, x25, x26, x27, x28, x29, x30;
    unsigned long sp;
    unsigned long pc;
    unsigned long spsr;
} TaskContext;

typedef struct {
    TaskContext context;
    unsigned char stack[TASK_STACK_SIZE];
    int state;
    int id;
    const char* name;
} Task;

void sched_init();
int  task_create(const char* name, void (*entry)());
void task_yield();
void schedule();
void sched_stats();

#endif
