#ifndef SYSCALL_H
#define SYSCALL_H

#define SYS_WRITE   1
#define SYS_READ    2
#define SYS_MALLOC  3
#define SYS_FREE    4
#define SYS_EXIT    5
#define SYS_GETPID  6
#define SYS_UPTIME  7

long syscall(long num, long a0, long a1, long a2);

#endif
