#ifndef IDT_H
#define IDT_H

#include <klib.h>
#include <stdint.h>
#include <stddef.h>

typedef struct {
    uint16_t off0;
    uint16_t sel;
    uint8_t zero;
    uint8_t type;
    uint16_t off1;
} __attribute__ ((packed)) idtdesc_t;

typedef struct {
    uint16_t limit;
    uint32_t base;
} __attribute((packed)) idtptr_t;

#define interrupts_on()                                                        \
  { __asm__ volatile("sti"); }
#define interrupts_off()                                                       \
  { __asm__ volatile("cli"); }

void irq_init();
void idt_init();
void irq_uninstall_handler(int);
void irq_install_handler(int, void (*)(regs_t *));
void idt_set_gate(uint8_t, uint32_t, uint16_t, uint8_t);

#endif
