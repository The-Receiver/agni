#ifndef PMM_H
#define PMM_H

#include <stdint.h>
#include <stddef.h>
#include <limits.h>

void pmm_init(void);
void *pmm_alloc_page(void);
void pmm_free_page(void *);
void *pmm_alloc(size_t);
void pmm_free(void *, size_t);

#endif
