#include <stdint.h>
#include <stddef.h>

typedef struct {
    char filename[100];
    char mode[8];
    char uid[8];
    char gid[8];
    char size[12];
    char mtime[12];
    char chksum[8];
    char typeflag[1];
} tarhdr_t;

uint64_t getsz(const char *inbuf)
{
    uint64_t sz = 0;
    size_t j = 0, count = 1;
    
    for(j = 11; j > 0; j--, count *= 8) {
        sz += ((inbuf[j - 1] - '0') * count);
    }
    
    return sz;
}
