#include <pmm.h>
#include <vmm.h>
#include <idt.h>
#include <tar.h>
#include <klib.h>
#include <video.h>
#include <paging.h>
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
    void *buf = pmm_alloc_page();
    void *buf2 = pmm_alloc_page();
    kputs("[boot] testing memory alloaction...\n");
    if((buf != NULL) && (buf2 != NULL)) {
        kprintf("[boot] first buffer allocated at %x \n", (uintptr_t)buf);
        kmemcpy(buf, "Safal", 6);
        kprintf("[boot] first buffer contains \"%s \"\n", buf);
        kprintf("[boot] second buffer alloacted at %x \n", (uintptr_t)buf2);
        kmemcpy(buf2, "Somil", 6);
        kprintf("[boot] second buffer contains \"%s \"\n", buf2);
    } else {
        kprintf("[boot] memory alloaction failed!\n");
    }
    kputs("[boot] loading initrd...\n");
    if(mboot->mods_count > 1) {
        kputs("[boot] FATAL: more than one module\n");
        asm volatile("hlt");
    } else if(!mboot->mods_count) {
        kputs("[boot] FATAL: no modules loaded!\n");
        asm volatile("hlt");
    }
    kprintf("[boot] initrd located at address %x \n", mboot->mods_addr);
    kprintf("[boot] first file is %s \n", *((uint32_t *)mboot->mods_addr));
    kprintf("[boot] first file contains:\n %s", *((uint32_t *)mboot->mods_addr) + 512);

    asm volatile("hlt");
}
