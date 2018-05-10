#ifndef TERMINAL_H
#define TERMINAL_H

#include <video.h>

void terminal_drawchar(char, uint16_t, uint16_t, uint32_t, uint32_t);
void terminal_putchar(char);
void terminal_puts(char *);

#endif
