#ifndef TERMINAL_H
#define TERMINAL_H

#include <video.h>

typedef enum {
    BLACK = 0,
    BLUE = 1,
    GREEN = 2,
    CYAN = 3,
    RED = 4,
    MAGENTA = 5,
    BROWN = 6,
    LGREY = 7,
    DGREY = 8,
    LBLUE = 9,
    LGREEN = 10,
    LCYAN = 11,
    LRED = 12,
    LMAGENTA = 13,
    LBROWN = 14,
    WHITE = 15,
} colour_t;

void terminal_drawchar(char, uint8_t, uint8_t, uint8_t);
void terminal_putchar(char);
void terminal_puts(char *);

#endif
