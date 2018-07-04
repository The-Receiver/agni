#ifndef VMM_H

#include <klib.h>
#include <paging.h>
#include <pmm.h>

void *vmm_alloc(size_t, int user);
void vmm_free(void *, size_t);
void *vmm_realloc(void *, size_t, int user);
int map_page(uint32_t *, void *, void *, int);
int unmap_page(uint32_t *, void *);

#endif
