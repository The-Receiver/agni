#include <pmm.h>
#include <vmm.h>
#include <idt.h>
#include <klib.h>
#include <video.h>
#include <paging.h>
#include <initrd.h>
#include <cpuid_kern.h>

void kmain(multiboot_info_t *mboot)
{
    idt_init();
    video_init();
    pmm_init();
    kputs("agni version 0.0.1\n");
    kputs("starting boot...\n");
    char cpuname[12 * 4];
    cpuid_get_name(cpuname);
    kprintf("cpu: %s \n", cpuname);
    kprintf("lower memory start: %x \n", mboot->mem_lower);
    kprintf("upper memory start: %x \n", mboot->mem_upper);
    kputs("[boot] interrupts initialized; testing the timer...");
    kdelay(3);
    kputs(" working!\n");
    kputs("[boot] loading initrd...\n");
    if(mboot->mods_count > 1) {
        kputs("[boot] FATAL: more than one module\n");
        asm volatile("hlt");
    } else if(!mboot->mods_count) {
        kputs("[boot] FATAL: no modules loaded!\n");
        asm volatile("hlt");
    }
    kprintf("[boot] initrd located at address %x \n", mboot->mods_addr);
    
    initrd_install(mboot->mods_addr);
    int handle = initrd_open("about", 0);
    if(handle == -1) {
        kprintf("[boot] failed to open test file\n");
    } else {
        char *file = pmm_alloc_page();
        initrd_read(handle, file, 21);
        kprintf("%s \n", file);
    }

    asm volatile("hlt");
}
