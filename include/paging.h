#ifndef PAGING_H
#define PAGING_H

#include <stdint.h>
#include <stddef.h>

void pmm_init(void);
void set_up_page_tables(void);
void paging_init(void);
void *pmm_alloc_page(void);
void pmm_free_page(void *);

#endif
