#ifndef PIT_H
#define PIT_H

#include <idt.h>
#include <klib.h>
#include <stdint.h>

void sched_init(void);
uint64_t pit_ticks();
uint64_t pit_secs();
void pit_handler(regs_t * r);

typedef struct {
    int free;
    uint32_t eax, ebx, ecx, edx, esi, edi, esp, eip, ebp, ds, cs, eflags,
        useresp, ss;
} thread_t;

typedef struct {
    int free;
    char name[16];
    uint32_t cr3;
    thread_t *threads;
} process_t;

int sched_create(char *, regs_t *, uint32_t);
int sched_create_thread(int, regs_t *, size_t);

#endif
