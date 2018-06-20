#ifndef VMM_H

#include <klib.h>
#include <paging.h>
#include <pmm.h>

void *vmm_alloc(size_t, int user);
void vmm_free(void *, size_t);
void *vmm_realloc(void *, size_t, int user);

#endif
