#ifndef CONF_H
#define CONF_H

#include <stdint.h>

typedef struct {
    uint8_t key[16];
    uint8_t value[16];
} conf_t;

#endif
