#include <pmm.h>
#include <vmm.h>
#include <vfs.h>
#include <idt.h>
#include <klib.h>
#include <video.h>
#include <paging.h>
#include <initrd.h>
#include <cpuid_kern.h>

void lol()
{
    *((uint8_t *)0xb800) = 'b';
    *((uint8_t *)0xb800 + 2) = 'b';
}

void kmain(multiboot_info_t * mboot)
{
    idt_init();
	video_init();
    
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
    
	char cpuname[12 * 4];
	cpuid_get_name(cpuname);
	kprintf("[boot] cpu: %s \n", cpuname);
    kprintf("[boot] mem: %u  megabytes of RAM\n", (mboot->mem_lower + mboot->mem_upper) / 1024);

	if (mboot->mods_count > 1) {
		kputs("[boot] FATAL: more than one module\n");
		asm volatile ("hlt");
	} else if (!mboot->mods_count) {
		kputs("[boot] FATAL: no modules loaded!\n");
		asm volatile ("hlt");
	}
	kputs("[boot] displaying files \"welcome\" and \"about\" from the initrd\n");
	int handle_welcome = vfs_open("0:docs/welcome", 0);
	if (handle_welcome == -1) {
		kprintf("[boot] failed to open test file\n");
	} else {
		char *welcome = vmm_alloc(0x1000, 0);
		vfs_read(handle_welcome, welcome, 211);
		kprintf("[boot] welcome: %s", welcome);
		vfs_close(handle_welcome);
        vmm_free(welcome, 0x1000);
    }

	int handle_about = vfs_open("0:docs/about", 0);
	if (handle_about == -1) {
		kprintf("[boot] failed to open test file\n");
	} else {
		char *about = vmm_alloc(0x1000, 0);
		vfs_read(handle_about, about, 37);
		kprintf("[boot] about: %s", about);
		vfs_close(handle_about);
        vmm_free(about, 0x1000);
	}

    interrupts_on();
    
	asm volatile ("hlt");
}
