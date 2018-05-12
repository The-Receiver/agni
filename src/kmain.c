#include <idt.h>
#include <video.h>
#include <paging.h>
#include <terminal.h>

void kmain(multiboot_info_t *mboot)
{
    video_init(mboot);
    terminal_puts("Agni version 0.0.1\n");
    terminal_puts("Starting boot...\n");
    set_up_page_tables();
    paging_init();
    terminal_puts("[info] activated identity paging\n");
    idt_init();
    terminal_puts("[info] set up interrupts\n");
    pmm_init();
    terminal_puts("[info] allocating memory...\n");;
    char *buf = pmm_alloc_page();
    if(buf != NULL) {
        kprintf("[info] memory allocated at address %x ! \n", (uintptr_t)buf);
        kputs("[info] copying Safal's keyboard mashing to buffer\n");
        kmemcpy(buf, "eorkeporjpeojrpejrepreporjeporjoerjoe", kstrlen("eorkeporjpeojrpejrepreporjeporjoerjoe"));
        kputs("[info] memory contains \"");
        kputs(buf);
        kputs("\"\n");
        
    }
    
}
