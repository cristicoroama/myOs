#ifndef PROCESS_H
#define PROCESS_H

#define MAX_PROCESSES   8
#define PROC_STACK_SIZE 8192

#define PROC_RUNNING    0
#define PROC_READY      1
#define PROC_BLOCKED    2
#define PROC_DEAD       3

typedef struct {
    int           pid;
    const char*   name;
    int           state;
    unsigned long stack[PROC_STACK_SIZE / 8];
    unsigned long sp;
    unsigned long pc;
    int           exit_code;
} Process;

void  proc_init();
int   proc_create(const char* name, void (*entry)());
void  proc_exit(int code);
void  proc_kill(int pid);
void  proc_list();
int   proc_getpid();

#endif
