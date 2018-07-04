#include <idt.h>
#include <initrd.h>
#include <klib.h>
#include <paging.h>
#include <pit.h>
#include <pmm.h>
#include <vfs.h>
#include <elf.h>
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
    
    kputs("[boot] testing elf loader...\n");
    
    elf_exec_t *program = elf_exec("0:bin/first_program");
    kprintf("[boot] program pd resides at %x \n", program->page_directory);
    kprintf("[boot] program entry is at virtual address %x \n", program->entry);
    
    schedule = 1;
    vmm_free(program, 1);

    for (;;) asm volatile ("cli; hlt");
}
