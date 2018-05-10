#include <video.h>
#include <terminal.h>

uint8_t bpp;
uint32_t *framebuffer;
uint16_t width, height, pitch;
static size_t x = 0, y = 0;
extern uint8_t bitmap_font[16 * 256];

void video_init(multiboot_info_t *mboot)
{
    vbe_mode_t *vbe = (vbe_mode_t *)mboot->vbe_mode_info;
    bpp = vbe->bpp;
    width = vbe->width;
    height = vbe->height;
    pitch = vbe->pitch;
    uintptr_t framebuffer_ptr = (uintptr_t)vbe->framebuffer;
    framebuffer = (uint32_t *)framebuffer_ptr;
    
    for(size_t i = 0; i < width * height; i++) {
        framebuffer[i] = 0x00000000;
    }
}

void video_putpixel(uint16_t xpos, uint16_t ypos, uint32_t colour)
{
    framebuffer[ypos * (pitch / 4) + xpos] = colour;
}

void terminal_drawchar(char c, uint16_t xpos, uint16_t ypos, uint32_t fg, uint32_t bg)
{
    uint16_t orig_x = xpos;
    
    for(uint8_t i = 0; i < 16; i++) {
        for(uint8_t j = 0; j < 8; j++) {
            if((bitmap_font[c * 16 + i] >> (7 - j)) & 1) {
                video_putpixel(xpos++, ypos, fg);
            } else {
                video_putpixel(xpos++, ypos, bg);
            }
        }
        ypos++;
        xpos = orig_x;
    }
}

void terminal_putchar(char c)
{
    if(c == '\n') {
        x = 0;
        y++;
        return;
    }
    if(x == width / 8) {
        x = 0;
        y++;
        return;
    }
    terminal_drawchar(c, x * 8, y * 16, 0xffa8a8a8, 0x00000000);
    x++;
}

void terminal_puts(char *s)
{
    for(; *s; s++) {
        terminal_putchar(*s);
    }
}
