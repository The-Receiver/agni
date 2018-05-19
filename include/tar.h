#ifndef TAR_H
#define TAR_H

#include <stdint.h>
#include <stddef.h>

typedef struct {
    char filename[100];
    char filemode[8];
    char fileUID[8];
    char filiGID[8];
    char size[12];
    char time[12];
    char checksum[8];
    uint8_t link;
} tarhdr_t;

void tar_init(tarhdr_t **, void *);

#endif
