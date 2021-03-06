#include <klib.h>

char *exception_messages[] = { "Division By Zero",
    "Debug",
    "Non Maskable Interrupt",
    "Breakpoint",
    "Into Detected Overflow",
    "Out of Bounds",
    "Invalid Opcode",
    "No Coprocessor",

    "Double Fault",
    "Coprocessor Segment Overrun",
    "Bad TSS",
    "Segment Not Present",
    "Stack Fault",
    "General Protection Fault",
    "Page Fault",
    "Unknown Interrupt",

    "Coprocessor Fault",
    "Alignment Check",
    "Machine Check",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",

    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved"
};

void isr_handler(regs_t * r)
{
    kprintf("Recieved exception %x : %s \n", r->int_no,
        exception_messages[r->int_no]);
    if (r->int_no == 0x0e) {
        size_t addr;
        __asm__ volatile (
            "mov %%cr2, %0"
            :"=r" (addr)
        );
        kprintf("Page fault at address %x \n", addr);
    }
    kputs("System halted!\n");
    __asm__ volatile ("cli; hlt");
}
