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
    kputs("[boot] loading initrd...\n");
    if(mboot->mods_count > 1) {
        kputs("[boot] FATAL: more than one module\n");
        asm volatile("hlt");
    } else if(!mboot->mods_count) {
        kputs("[boot] FATAL: no modules loaded!\n");
        asm volatile("hlt");
    }
    uint32_t *initrd_ptr = (uint32_t *) mboot->mods_addr;
    kprintf("[boot] initrd located at address %x \n", initrd_ptr);
    tar_mount(initrd_ptr);
    tarFILE *f;
    kputs("[boot] testing initrd driver by opening file \"hello.c\"...\n");
    if((f = tar_open("hello.c")) != NULL) {
        kputs("[boot] file hello.c found!\n");
        char *file = pmm_alloc_page();
        tar_read(file, f, 87);
        kprintf("[boot] file hello.c contains: \n %s \n", file);
    } else {
        kputs("[boot] opening test file failed\n");
    }

    asm volatile("hlt");
}
