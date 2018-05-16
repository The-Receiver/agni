#include <vmm.h>

void *vmm_alloc(size_t n)
{
    n += 4096;
    return pmm_alloc_page();
}
