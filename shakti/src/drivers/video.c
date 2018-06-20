#include <klib.h>
#include <terminal.h>
#include <video.h>

uint8_t bpp;
uint8_t terminal_colour;
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

void terminal_set_colour(colour_t fg, colour_t bg)
{
    terminal_colour = vga_entry_colour(fg, bg);
}

void video_init()
{
    framebuffer = (uint16_t *) 0xb8000;
    width = 80;
    height = 25;
}

void update_cursor(int x, int y)
{
    uint16_t pos = y * width + x;

    outb(0x3D4, 0x0F);
    outb(0x3D5, (uint8_t) (pos & 0xFF));
    outb(0x3D4, 0x0E);
    outb(0x3D5, (uint8_t) ((pos >> 8) & 0xFF));
}

void terminal_scrolldown()
{
    for (size_t index = (80 * 1); index < 80 * 25; index++) {
        framebuffer[index - 80] = framebuffer[index];
    }

    for (size_t index = 0; index < 80; index++) {
        framebuffer[index + (80 - 1) * 80] =
            vga_entry(' ', vga_entry_colour(LGREY, BLACK));
    }
}

void terminal_drawchar(char c, uint8_t xpos, uint8_t ypos, uint8_t colour)
{
    size_t index = ypos * width + xpos;
    framebuffer[index] = vga_entry(c, colour);
}

void terminal_putchar(char c)
{
    if (!c) {
        return;
    }
    if (c == '\n') {
        x = 0;
        y++;
        return;
    }
    if (y == 25) {
        terminal_scrolldown();
    }
    terminal_drawchar(c, x, y, vga_entry_colour(LGREY, BLACK));
    update_cursor(x, y);
    x++;
}

void terminal_puts(char *s)
{
    for (; *s; s++) {
        terminal_putchar(*s);
    }
}
