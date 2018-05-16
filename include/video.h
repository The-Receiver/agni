#ifndef VIDEO_H
#define VIDEO_H

#include <stdint.h>
#include <stddef.h>
#include <multiboot.h>

typedef struct vbe_mode_t
{
    uint16_t attributes;
    uint8_t window_a;
    uint8_t window_b;
    uint16_t granularity;
    uint16_t window_size;
    uint16_t segment_a;
    uint16_t segment_b;
    uint32_t win_func_ptr;

    uint16_t pitch;
    uint16_t width;
    uint16_t height;

    uint8_t w_char;
    uint8_t y_char;
    uint8_t planes;
    uint8_t bpp;
    uint8_t banks;

    uint8_t memory_model;
    uint8_t bank_size;
    uint8_t image_pages;
    uint8_t reserved0;

    uint16_t red;
    uint16_t green;
    uint16_t blue;
    uint16_t reserved_mask;
    uint8_t direct_color;

    uint32_t framebuffer;
    uint32_t off_screen_mem;
    uint32_t off_screen_mem_size;

    uint8_t reserved1[206];
}__attribute__((packed)) vbe_mode_t;

void video_init();
void video_putpixel(uint16_t, uint16_t, uint32_t);

#endif
