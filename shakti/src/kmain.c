#include <pmm.h>
#include <vmm.h>
#include <vfs.h>
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
    kputs("[boot] loading initrd...\n");
    multiboot_module_t *p = (multiboot_module_t *)mboot->mods_addr;
    uint32_t initrd_start = p->mod_start;
    kprintf("[boot] initrd located at address %x \n", initrd_start);
    
    vfs_init();
    initrd_install(initrd_start);
    vfs_mount("initrd", 0);
    
    char cpuname[12 * 4];
    cpuid_get_name(cpuname);
    kprintf("[boot] cpu: %s \n", cpuname);
    kprintf("[boot] lower memory start: %x \n", mboot->mem_lower);
    kprintf("[boot] upper memory start: %x \n", mboot->mem_upper);
    kputs("[boot] interrupts initialized; testing the timer...");
    kdelay(3);
    kputs(" working!\n");
    if(mboot->mods_count > 1) {
        kputs("[boot] FATAL: more than one module\n");
        asm volatile("hlt");
    } else if(!mboot->mods_count) {
        kputs("[boot] FATAL: no modules loaded!\n");
        asm volatile("hlt");
    }
    kputs("[boot] displaying files \"welcome\" and \"about\" from the initrd\n");
    int handle_welcome = vfs_open("0:docs/welcome", 0);
    if(handle_welcome == -1) {
        kprintf("[boot] failed to open test file\n");
    } else {
        char *file = pmm_alloc_page();
        vfs_read(handle_welcome, file, 21);
        kputs(file);
        vfs_close(handle_welcome);
        pmm_free_page(file);
    }
    
    int handle_about = vfs_open("0:docs/about", 0);
    if(handle_about == -1) {
        kprintf("[boot] failed to open test file\n");
    } else {
        char *file = pmm_alloc_page();
        vfs_read(handle_about, file, 37);
        kputs(file);
        vfs_close(handle_about);
        pmm_free_page(file);
    }
    
    asm volatile("hlt");
}
