#include <klib.h>
#include <pit.h>
#include <vmm.h>

uint64_t ticks = 0;
uint64_t seconds = 0;
int schedule = 0;

process_t *processes;
extern void context_switch();

void sched_enable(void)
{
    schedule = 1;
}

int find_free_process()
{
    for (size_t i = 0; i < 512; i++) {
        if (processes[i].free) {
            processes[i].free = 0;
            return i;
        }
    }
    return -1;
}

int find_free_thread(int pid)
{
    for (size_t i = 0; i < 512; i++) {
        if (processes[pid].threads[i].free) {
            processes[pid].threads[i].free = 0;
            return i;
        }
    }
    return -1;
}

void pit_freq(uint32_t freq)
{
    uint16_t x = 1193182 / freq;
    if ((1193182 % freq) > (freq / 2))
        x++;

    outb(0x40, (uint8_t) (x & 0x00ff));
    outb(0x40, (uint8_t) (x / 0x0100));
}

void sched_init(void)
{
    pit_freq(1000);

    processes = vmm_alloc(3, 0);

    for (size_t i = 0; i < 512; i++) {
        processes[i].free = 1;
        processes[i].threads = vmm_alloc(8, 0);

        if (processes[i].threads == NULL) {
            kputs("failed alloc lol\n");
            for (;;) ;
        }

        for (size_t j = 0; j < 512; j++) {
            processes[i].threads[j].free = 1;
        }
    }

    kmemcpy(processes[0].name, "shakti", 7);
    processes[0].free = 0;

    asm volatile (
        "mov %%cr3, %0"
        :"=a" (processes[0].cr3)
    );

    processes[0].threads[0].free = -1;

    irq_install_handler(0, pit_handler);
}

int sched_create_thread(int pid, regs_t * r, size_t stksz)
{
    size_t *stk;

    while (stksz % 0x1000)
        stksz++;
    size_t asz = stksz / 0x1000;

    stk = (pid == 0) ? vmm_alloc(asz, 0) : vmm_alloc(asz, 1);

    if (stk == NULL) {
        return -1;
    }

    int tid = find_free_thread(pid);
    if (tid == -1) {
        return -1;
    }
    processes[pid].threads[tid].cs = r->cs;
    processes[pid].threads[tid].ds = r->ds;
    processes[pid].threads[tid].eflags = r->eflags;
    processes[pid].threads[tid].ss = r->ss;
    processes[pid].threads[tid].eax = r->eax;
    processes[pid].threads[tid].ebx = r->ebx;
    processes[pid].threads[tid].ecx = r->ecx;
    processes[pid].threads[tid].edx = r->edx;
    processes[pid].threads[tid].esi = r->esi;
    processes[pid].threads[tid].edi = r->esi;
    processes[pid].threads[tid].ebp = r->ebp;
    processes[pid].threads[tid].esp = (uint32_t) stk;
    processes[pid].threads[tid].useresp = (uint32_t) stk;
    return tid;
}

int sched_create(char *name, regs_t * r, uint32_t cr3)
{
    int pid = find_free_process();
    if (pid == -1) {
        return -1;
    }
    kmemcpy(processes[pid].name, name, kstrlen(name));
    int main = sched_create_thread(pid, r, 4096);
    processes[pid].cr3 = cr3;

    return (main == -1) ? main : pid;
}

void pit_handler(regs_t * r)
{
    ticks++;
    if (!schedule) {
        return;
    }
    if (ticks % 10) {
        for (size_t i = 0;; i++) {
            if (i == 511)
                i = 0;

            if (processes[i].free)
                continue;

            for (size_t j = 0; j < 512; j++) {
                if (processes[i].threads[j].free)
                    continue;

                if (processes[i].threads[j].free == -1)
                    for (;;) ;

                uint32_t pd = processes[i].cr3;

                asm volatile (
                    "mov %0, %%eax"
                    :
                    :"r" (pd)
                );

                regs_t r;
                r.eax = processes[i].threads[j].eax;
                r.ebx = processes[i].threads[j].ebx;
                r.ecx = processes[i].threads[j].ecx;
                r.edx = processes[i].threads[j].edx;
                r.esi = processes[i].threads[j].esi;
                r.edi = processes[i].threads[j].edi;
                r.ebp = processes[i].threads[j].ebp;
                r.useresp = processes[i].threads[j].useresp;
                r.cs = processes[i].threads[j].cs;
                r.ds = processes[i].threads[j].ds;
                r.eflags = processes[i].threads[j].eflags;

                asm volatile (
                    "mov %0, %%ebx"
                    :
                    :"r" (&r)
                );

                context_switch();
            }
        }
    }
}

uint64_t pit_ticks(void)
{
    return ticks;
}

uint64_t pit_secs(void)
{
    return seconds;
}
