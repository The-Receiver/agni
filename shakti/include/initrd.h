#ifndef TAR_H
#define TAR_H

#include <klib.h>
#include <stdint.h>
#include <stddef.h>

typedef struct {
    char name[100];
    char mode[8];
    char uid[8];
    char gid[8];
    char size[12];
    char mtime[12];
    char chksum[8];
    char typeflag;
    char linkname[100];
    char magic[6];
    char version[2];
    char uname[32];
    char gname[32];
    char devmajor[8];
    char devminor[8];
    char prefix[155];
} __attribute__((packed))tarhdr_t;

typedef struct {
    int perms;
    int used;
    size_t offset;
    uint8_t *start;
} tar_handle_t;

int initrd_open(char *, int);
int initrd_read(int, void *, size_t);
void initrd_install(uint32_t);

#endif
