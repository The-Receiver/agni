#ifndef PAGING_H
#define PAGING_H

#include <stdint.h>
#include <stddef.h>

#define LOAD_ADDRESS 0x00100000
#define HIGHER_HALF_ADDRESS 0xC0100000


void set_up_page_tables(void);
void paging_init(void);

#endif
