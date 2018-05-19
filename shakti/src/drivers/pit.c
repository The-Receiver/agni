#include <pit.h>
#include <klib.h>

uint64_t ticks = 0;
uint64_t seconds = 0;

void pit_init(void)
{
    irq_install_handler(0, pit_handler);
}

void pit_handler(regs_t *r)
{
    int n = r->int_no;
    n += 5;
    ticks++;
    if((ticks % 18) == 0) {
        seconds++;
    }
}

uint64_t pit_ticks()
{
    return ticks;
}

uint64_t pit_secs()
{
    return seconds;
}
