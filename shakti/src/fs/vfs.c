#include <pmm.h>
#include <vfs.h>
#include <klib.h>

static vfs_fs_t *filesystems;

static int find_free_fs()
{
    for(int i = 0; i < 64; i++) {
        if(filesystems[i].open == NULL) {
            return i;
        }
    }
    return -1;
}

void vfs_init()
{
    filesystems = pmm_alloc_page();
    for(size_t i = 0; i < 64; i++) {
        filesystems[i].open = NULL;
    }
}

int vfs_install_fs(char *name, int (*open)(char *), int (*close)(int), int (*read)(int, void *, int), int (*write)(int, void *, int), int (*stat)(char *, void *), int (*fstat)(int, char *, void *), int (*lseek)(int, int, int), int (*mount)(char *, char *, char *, long, void *))
{
    int index = find_free_fs();
    if(index == -1) {
        return -1;
    }
    kmemcpy(filesystems[index].name, name, 32);
    filesystems[index].open = open;
    filesystems[index].close = close;
    filesystems[index].read = read;
    filesystems[index].write = write;
    filesystems[index].stat = stat;
    filesystems[index].fstat = fstat;
    filesystems[index].lseek = lseek;
    filesystems[index].mount = mount;
}
