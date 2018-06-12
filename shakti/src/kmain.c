#include <pmm.h>
#include <vmm.h>
#include <vfs.h>
#include <idt.h>
#include <klib.h>
#include <sched.h>
#include <video.h>
#include <paging.h>
#include <initrd.h>
#include <cpuid_kern.h>

extern uint32_t *pd;
extern uint32_t start;
extern uint32_t stack_top;

void kmain(multiboot_info_t * mboot)
{
	idt_init();
	video_init();
	pmm_init();
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
	kprintf("[boot] lower memory start: %x \n", mboot->mem_lower);
	kprintf("[boot] upper memory start: %x \n", mboot->mem_upper);
	kputs("[boot] interrupts initialized; testing the timer...");
	kdelay(3);
	kputs(" working!\n");
	if (mboot->mods_count > 1) {
		kputs("[boot] FATAL: more than one module\n");
		asm volatile ("hlt");
	} else if (!mboot->mods_count) {
		kputs("[boot] FATAL: no modules loaded!\n");
		asm volatile ("hlt");
	}
	kputs
	    ("[boot] displaying files \"welcome\" and \"about\" from the initrd\n");
	int handle_welcome = vfs_open("0:docs/welcome", 0);
	if (handle_welcome == -1) {
		kprintf("[boot] failed to open test file\n");
	} else {
		char *file = vmm_alloc(1);
        kprintf("[boot] memory for welcome allocated at %x \n", file);
		vfs_read(handle_welcome, file, 211);
		kprintf("[boot] welcome: %s", file);
		vfs_close(handle_welcome);
		vmm_free(file, 1);
    }

	int handle_about = vfs_open("0:docs/about", 0);
	if (handle_about == -1) {
		kprintf("[boot] failed to open test file\n");
	} else {
		char *file = vmm_alloc(1);
        kprintf("[boot] memory for about allocated at %x \n", file);
		vfs_read(handle_about, file, 80);
		kprintf("[boot] about: %s", file);
		vfs_close(handle_about);
		vmm_free(file, 1);
	}

	kputs("[boot] starting scheduler...\n");
	sched_init();

	asm volatile ("hlt");
}
