#include <klib.h>
#include <paging.h>
#include <limits.h>

#define PAGE_SIZE 0x1000
#define LOAD_ADDRESS 0x00100000
#define HIGHER_HALF_ADDRESS 0xC0100000

uint32_t pd[1024] __attribute__((aligned(PAGE_SIZE)));
uint32_t pt[1024 * 1024] __attribute__((aligned(PAGE_SIZE)));;

void set_up_page_tables(void)
{
    uint32_t *pt_pa = (uint32_t *)((char *)pt - HIGHER_HALF_ADDRESS + LOAD_ADDRESS);
    uint32_t *pd_pa = (uint32_t *)((char *)pd - HIGHER_HALF_ADDRESS + LOAD_ADDRESS);
    
    
    for(size_t i = 0; i < 1024; i++) {
        pt_pa[i] = (i * PAGE_SIZE) | 0x03;
    }
    for(size_t i = 0; i < 1024 * 1024; i++) {
        pt_pa[i] = (i * PAGE_SIZE) | 0x03;
    }
    for(size_t i = 0; i < 1024; i++) {
        pd_pa[i] = (uint32_t) (&pt_pa[i * 1024]) | 0x03;
    }
}
