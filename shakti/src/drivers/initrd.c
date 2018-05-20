#include <pmm.h>
#include <initrd.h>

#define SEEK_SET 0
#define SEEK_CUR 1
#define SEEK_END 2

static uint8_t *archive;
static initrd_handle_t *handles;

typedef signed long int ssize_t;

static uint32_t atoi_octal(char *in) {

    uint32_t size = 0;
    uint32_t j;

    uint32_t count = 1;

    for (j = 11; j > 0; j--, count *= 8)
        size += ((in[j - 1] - '0') * count);

    return size;

}

static void alloc_handles()
{
    handles = pmm_alloc_page();
    for(size_t i = 0; i < 128; i++) {
        handles[i].used = 0;
    }
}

static ssize_t find_free_handle()
{
    for(size_t i = 0; i < 128; i++) {
        if(!handles[i].used) {
            return i;
        }
    }
    return -1;
}

int initrd_open(char *path, int perms)
{
    tarhdr_t *ptr = (tarhdr_t *)archive;
    
    while(kstrcmp(ptr->magic, "ustar") == 0) {
        size_t sz = atoi_octal(ptr->size);
        if(kstrcmp(ptr->name, path) == 0) {
            ssize_t free_handle = find_free_handle();
            if(free_handle < 0) {
                return -1;
            }
            handles[free_handle].used = 1;
            handles[free_handle].offset = 0;
            handles[free_handle].size = sz;
            handles[free_handle].perms = perms;
            handles[free_handle].start = (uint8_t *)ptr + 512;
            return free_handle;
        }
        ptr += (((sz + 511) / 512) + 1) * 512;
    }
    return -1;
}

int initrd_read(int handle, void *buf, size_t n)
{
    if(handle == -1) {
        return -1;
    }
    kmemcpy(buf, handles[handle].start + handles[handle].offset, n);
    return n;
}

size_t initrd_lseek(int handle, size_t offset, int whence)
{
    switch(whence) {
        case SEEK_SET:
            handles[handle].offset = offset;
        break;
        
        case SEEK_CUR:
            handles[handle].offset = (handles[handle].offset + offset);
        break;
        
        case SEEK_END:
            handles[handle].offset = (handles[handle].size + offset);
        break;
    }
    return offset;
}

int intird_close(int handle)
{
    if(!handles[handle].used) {
        return -1;
    }
    kmemset(&handles[handle], 0, sizeof(initrd_handle_t));
    return 0;
}

void initrd_install(uint32_t addr)
{
    alloc_handles();
    void *interim = (void *)addr;
    archive = (uint8_t *)interim;
}


