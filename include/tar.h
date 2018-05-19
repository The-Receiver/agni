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
    char typeflag;
} __attribute__((packed))tarhdr_t;


typedef struct {
    size_t size;
    void *offset;
    uint32_t *archive_offset;
} tarFILE;

void tar_mount(uint32_t *);
tarFILE *tar_open(char *);
void tar_read(void *, tarFILE *, size_t);

#endif
