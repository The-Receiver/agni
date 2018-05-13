#include <pmm.h>
#include <idt.h>
#include <klib.h>
#include <video.h>
#include <paging.h>
#include <terminal.h>

void kmain(multiboot_info_t *mboot)
{
    video_init(mboot);
    kputs("Agni version 0.0.1\n");
    kputs("Starting boot...\n");
    set_up_page_tables();
    kputs("[boot] setting up identity paging...\n");
    paging_init();
    kputs("[boot] activated identity paging\n");
    kputs("[boot] setting up interrupts...\n");
    idt_init();
    kputs("[boot] activated interrupts\n");
    pmm_init();
    kputs("[boot] allocating memory...\n");;
    char *buf = pmm_alloc(1000);    /* allocate 1000 pages (40 MiB) as a test */
    if(buf != NULL) {
        kprintf("[boot] memory allocated at address %x \n", (uintptr_t)buf);
        kputs("[boot] copying data to the address...\n");
        kmemcpy(buf, "eorkeporjpeojrpejrepreporjeporjoerjoe", kstrlen("eorkeporjpeojrpejrepreporjeporjoerjoe"));
        kputs("[boot] memory contains \"");
        kputs(buf);
        kputs("\"\n");
        pmm_free(buf, 1000); /* free the allocated memory */
    } else {
        kputs("[boot] allocating memory failed\n");
    }
    kputs("[boot] testing the pit...\n");
    kdelay(2);
    kputs("[boot] working!\n");
    asm volatile("hlt");
}
