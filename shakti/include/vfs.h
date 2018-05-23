#ifndef VFS_H
#define VFS_H

#include <stdint.h>
#include <stddef.h>

typedef struct {
    char name[32];
    int (*open)(char *);
    int (*close)(int);
    int (*read)(int, void *, int);
    int (*write)(int, void *, int);
    int (*stat)(char *, void *);
    int (*fstat)(int, char *, void *);
    int (*lseek)(int, int, int);
    int (*mount)(char *, char *, char *, long, void *);
} vfs_fs_t;

typedef struct {
    char name[12];
    size_t fs_index;
} vfs_index_t;

typedef struct {
    char name[12];
    size_t index_index;
} vfs_mountpoint_t;

#endif
