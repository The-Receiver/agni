#include <video.h>
#include <terminal.h>

uint8_t bpp;
uint16_t *framebuffer;
uint16_t width, height, pitch;
static size_t x = 0, y = 0;
extern uint8_t bitmap_font[16 * 256];

static inline uint8_t vga_entry_colour(colour_t fg, colour_t bg) 
{
	return fg | bg << 4;
}
 
static inline uint16_t vga_entry(uint8_t uc, uint8_t colour) 
{
	return (uint16_t) uc | (uint16_t) colour << 8;
}


void video_init()
{
    framebuffer = (uint16_t *) 0xb8000;
    width = 80;
    height = 25;
}



void terminal_drawchar(char c, uint8_t xpos, uint8_t ypos, uint8_t colour)
{
    size_t index = ypos * width + xpos;
    framebuffer[index] = vga_entry(c, colour);
}

void terminal_putchar(char c)
{
    if(!c) {
        return;
    }
    if(c == '\n') {
        x = 0;
        y++;
        return;
    }
    terminal_drawchar(c, x, y, vga_entry_colour(LGREY, BLACK));
    x++;
}

void terminal_puts(char *s)
{
    for(; *s; s++) {
        terminal_putchar(*s);
    }
}
