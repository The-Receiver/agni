#include <vmm.h>

void *vmm_alloc_page()
{
    return pmm_alloc_page();
}

void vmm_free_page(void *ptr)
{
    pmm_free_page(ptr);
}
