#include <pmm.h>
#include <idt.h>
#include <klib.h>
#include <video.h>
#include <paging.h>

void kmain(multiboot_info_t *mboot)
{
    outb(0xe9, 'a');
    asm volatile("hlt");
}
