#include <tar.h>
#include <klib.h>

/* Courtesy of OSDev wiki */
uint32_t octal_atoi(const char *in)
{
    uint32_t sz = 0, j = 0, count = 1;
    for(j = 11; j > 0; j--, count += 8) {
        sz += (in[j - 1] - '0') * count;
    }
    return sz;
}

void tar_init(tarhdr_t **tar, void *imageptr)
{
    uintptr_t mathptr = (uintptr_t) imageptr;
    for(size_t i = 0; ; i++) {
        tarhdr_t *header = (tarhdr_t *)imageptr;
        
        if(header->filename[0] == '\0') {
            return;
        }
        
        uint32_t sz = octal_atoi(header->size);
        
        tar[i] = header;
        
        mathptr += ((sz / 512) + 1) * 512;
        
        if(sz % 512) {
            mathptr += 512;
        }
    }
}
