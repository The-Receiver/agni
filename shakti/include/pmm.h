#ifndef PMM_H
#define PMM_H

#include <stdint.h>
#include <stddef.h>
#include <limits.h>
#include <multiboot.h>

void pmm_init(multiboot_info_t *);
void *pmm_alloc_page(void);
void pmm_free_page(void *);
void *pmm_alloc(size_t);
void pmm_free(void *, size_t);

#endif
