#include <klib.h>
#include <paging.h>
#include <limits.h>

#define PAGE_SIZE 4096
#define KRNL_MEMORY_BASE (uintptr_t)0x1000000

uint32_t pd[1024] __attribute__((aligned(PAGE_SIZE)));
uint32_t pt[1024 * 1024] __attribute__((aligned(PAGE_SIZE)));

volatile uint8_t bitmap[(1024 * 1024) / CHAR_BIT] __attribute__((aligned(PAGE_SIZE)));

void set_up_page_tables(void)
{
    for(size_t i = 0; i < 1024 * 1024; i++) {
        pt[i] = (i * PAGE_SIZE) | 3;
    }
    for(size_t i = 0; i < 1024; i++) {
        pd[i] = (uint32_t) (&pt[i * 1024]) | 0x03;
    }
}

void pmm_init()
{
    kmemset((void *)bitmap, 0, (1024 * 1024) / CHAR_BIT);
    bitmap[0] = 1;
}

void *pmm_alloc_page()
{
    size_t x = 0;
    for(size_t i = 0; i < (1024 * 1024) / CHAR_BIT; i++) {
        for(size_t j = 0; j < CHAR_BIT; j++) {
            size_t index = i * CHAR_BIT + j;
            uint8_t bit = (bitmap[i] >> j) & 1;
            if(!bit) {
                bitmap[i] |= (1 << j);
                return (void *)(KRNL_MEMORY_BASE+(index * PAGE_SIZE));
            }
        }
    }
    return NULL;
}

void pmm_free_page(void *addr)
{
    addr -= KRNL_MEMORY_BASE;
    size_t bit = (uintptr_t) addr / PAGE_SIZE;
    for(size_t i = 0; i < (1024 * 1024) / CHAR_BIT; i++) {
        for(size_t j = 0; j < CHAR_BIT; j++) {
            if(((i * CHAR_BIT + j) * PAGE_SIZE) == bit) {
                bitmap[i] &= ~(1 << j);
                return;
            }
        }
    }
}
