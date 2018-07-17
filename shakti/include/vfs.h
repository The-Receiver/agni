#ifndef VFS_H
#define VFS_H

#include <stddef.h>
#include <stdint.h>

typedef struct {
    char name[32];
    int (*open) (char *, int);
    int (*close) (int);
    int (*read) (int, void *, size_t);
    int (*write) (int, void *, size_t);
    int (*stat) (char *, void *);
    int (*fstat) (int, void *);
    int (*lseek) (int, int, int);
    int (*mount) (int);
} vfs_fs_t;

typedef struct {
    char name[12];
    size_t fs_index;
} vfs_index_t;

typedef struct {
    int used;
    int parent_handle;
    vfs_fs_t *fs;
} vfs_handle_t;

typedef struct {
    int dev;
    vfs_fs_t *fs_ptr;
} vfs_mountpoint_t;

void vfs_init();
int vfs_mount(char *, int);
int vfs_open(char *, int);
int vfs_read(int, void *, size_t);
int vfs_fstat(int, void *);
int vfs_close(int);
int vfs_install_fs(char *name, int (*open) (char *, int), int (*close) (int),
           int (*read) (int, void *, size_t),
           int (*write) (int, void *, size_t),
           int (*stat) (char *, void *), int (*fstat) (int, void *),
           int (*lseek) (int, int, int), int (*mount) (int));

#endif
