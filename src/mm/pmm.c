#include <pmm.h>
#include <klib.h>

#define PAGE_SIZE 0x1000
#define KRNL_BASE 0x10000000

volatile uint8_t bitmap[(1024 * 1024) / CHAR_BIT] __attribute__((aligned(PAGE_SIZE)));

void pmm_init()
{
    for(size_t i = 0; i < (1024 * 1024) / CHAR_BIT; i++) {
        bitmap[i] = 0;
    }
    
    bitmap[0] = 1;
}

void *pmm_alloc_page()
{
    for(size_t i = 0; i < (1024 * 1024) / CHAR_BIT; i++) {
        for(size_t j = 0; j < CHAR_BIT; j++) {
            uint8_t bit = (bitmap[i] >> j) & 1;
            if(!bit) {
                bitmap[i] |= (1 << j);
                return (void *)(KRNL_BASE+((i*8)+j)*PAGE_SIZE);
            }
        }
    }
    return NULL;
}

void pmm_free_page(void *addr)
{
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
