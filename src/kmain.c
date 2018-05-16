#include <pmm.h>
#include <idt.h>
#include <klib.h>
#include <video.h>
#include <paging.h>

void kmain(multiboot_info_t *mboot)
{
    idt_init();
    video_init();
    pmm_init();
    kputs("agni version 0.0.1\n");
    kputs("starting boot...\n");
    kputs("[boot] interrupts initialized; testing the timer...");
    kdelay(3);
    kputs(" working!\n");
    kputs("[boot] testing physical memory manager...\n");
    void *ptr = pmm_alloc_page();
    void *ptr2 = pmm_alloc_page();
    if((ptr != NULL) && (ptr2 != NULL)) {
        kprintf("[boot] first buffer allocated at address %x \n", (uintptr_t)ptr);
        kmemcpy(ptr, "dsdsdsdsds", kstrlen("dsdsdsdsds"));
        kprintf("[boot] the address contains %s \n", ptr);
        kprintf("[boot] second buffer alloacted at address %x \n", (uintptr_t)ptr2);
        kmemcpy(ptr2, "jkdfpodjo", kstrlen("jkdfpodjo"));
        kprintf("[boot] the address contians %s \n", ptr2);
    } else {
        kputs("[boot] failed to allocate memory\n");
    }
    pmm_free_page(ptr);
    pmm_free_page(ptr2);
    asm volatile("hlt");
}
