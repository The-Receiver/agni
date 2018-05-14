#include <klib.h>

void isr_handler(regs_t *r)
{
    terminal_puts("Recieved an interrupt!");
    asm volatile ("\ncli\nhlt\n");
}
