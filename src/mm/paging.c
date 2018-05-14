#include <klib.h>
#include <paging.h>
#include <limits.h>

#define PAGE_SIZE 0x1000

uint32_t pd[1024] __attribute__((aligned(PAGE_SIZE)));
uint32_t pt[1024 * 1024] __attribute__((aligned(PAGE_SIZE)));;

void set_up_page_tables(void)
{
    for(size_t i = 0; i < 1024 * 1024; i++) {
        pt[i] = (i * PAGE_SIZE) | 3;
    }
    for(size_t i = 0; i < 1024; i++) {
        pd[i] = (uint32_t) (&pt[i * 1024]) | 0x03;
    }
}
