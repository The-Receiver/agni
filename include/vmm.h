#ifndef VMM_H

#include <pmm.h>
#include <klib.h>
#include <paging.h>

void *vmm_alloc_page(void);
void vmm_free_page(void *);

#endif
