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
    terminal_puts("[info] allocating memory...\n");
    pmm_init();
    if(pmm_alloc_page() != NULL) {
        terminal_puts("allocated memory!\n");
    } else {
        terminal_puts("allocation failed!\n");
    }
}
