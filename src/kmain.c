#include <video.h>
#include <terminal.h>

void kmain(multiboot_info_t *mboot)
{
    video_init(mboot);
    terminal_puts("Agni Version 0.0.1\n");
    terminal_puts("The Shakti Kernel is starting the boot procedure...\n");
}
