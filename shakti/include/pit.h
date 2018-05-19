#ifndef PIT_H
#define PIT_H

#include <idt.h>
#include <klib.h>
#include <stdint.h>

void pit_init(void);
uint64_t pit_ticks();
uint64_t pit_secs();
void pit_handler(regs_t *r);

#endif
