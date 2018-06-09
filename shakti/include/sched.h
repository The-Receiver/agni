#ifndef SCHED_H
#define SCHED_H

#include <stddef.h>
#include <stdint.h>

typedef struct {
    int attached;
    uint32_t eax, ebx,
             ecx, edx;
    uint32_t esp, eip;
} thread_t;

typedef struct {
    int attached;
    uint32_t cr3, ds;
    size_t nthreads;
    thread_t *threads;
    char name[20];
} process_t;

int sched_init();
int sched_create_thread(int, uint32_t, uint32_t, uint32_t, uint32_t, uint32_t, uint32_t);
int sched_create(char *, uint32_t, uint32_t, uint32_t, uint32_t, uint32_t, uint32_t, uint32_t, uint32_t);

#endif
