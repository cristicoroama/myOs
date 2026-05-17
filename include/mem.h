#ifndef MEM_H
#define MEM_H

void mem_init();
void* malloc(unsigned long size);
void free(void* ptr);
void mem_stats();

#endif
