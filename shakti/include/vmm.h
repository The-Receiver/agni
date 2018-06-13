#ifndef VMM_H

#include <pmm.h>
#include <klib.h>
#include <paging.h>

void *vmm_alloc(size_t);
void vmm_free(void *, size_t);
void *vmm_realloc(void *, size_t);

#endif
