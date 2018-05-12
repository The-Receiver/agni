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
    for(size_t i = 0; i < 80; i++) {
        kprintf("[info] allocated memory at address %x \n", (uintptr_t)pmm_alloc_page());
    }
}
