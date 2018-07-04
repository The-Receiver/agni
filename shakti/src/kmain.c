#include <idt.h>
#include <initrd.h>
#include <klib.h>
#include <paging.h>
#include <pit.h>
#include <pmm.h>
#include <vfs.h>
#include <video.h>
#include <vmm.h>

extern int schedule;

void kmain(multiboot_info_t * mboot)
{
    idt_init();
    video_init();

    interrupts_on();

    pmm_init(mboot);
    kputs("agni version 0.0.1\n");
    kputs("starting boot...\n");
    kputs("[boot] loading initrd...\n");

    multiboot_module_t *p = (multiboot_module_t *) mboot->mods_addr;
    uint32_t initrd_start = p->mod_start;
    kprintf("[boot] initrd located at address %x \n", initrd_start);
    vfs_init();
    initrd_install(initrd_start);
    vfs_mount("initrd", 0);

    kprintf("[boot] mem: %u  megabytes of RAM\n",
        (mboot->mem_lower + mboot->mem_upper) / 1024);
    
    schedule = 1;

    for (;;) asm volatile ("cli; hlt");
}
