#include <pmm.h>
#include <klib.h>

#define PAGE_SIZE 0x1000
#define KRNL_MEMORY_BASE (uintptr_t)0x1000000

uint8_t bitmap[(1024 * 1024) / CHAR_BIT] __attribute__((aligned(PAGE_SIZE)));

static uint8_t check_continuity(void **addresses, size_t naddresses)
{
    size_t x = 0;
    for(size_t i = 0; i < naddresses - 1; i++) {
        if((addresses[i + 1] - addresses[i]) != PAGE_SIZE) x++;
    }
    return (x == 0);
}

void pmm_init()
{
    kmemset(bitmap, 0, (1024 * 1024) / CHAR_BIT);
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

void *pmm_alloc(size_t n)
{
    uint8_t first = 1;
    void *addresses[n];
    void *old_addresses[n];
    for(;;) {
        if(first) {
            for(size_t i = 0; i < n; i++) {
                addresses[i] = pmm_alloc_page();
                if(addresses[i] == NULL) return NULL;
            }
            first = 0;
        }
        if(check_continuity(addresses, n)) return addresses[0];
        else {
            kmemcpy(old_addresses, addresses, n * sizeof(void *));
            for(size_t i = 0; i < n; i++) {
                addresses[i] = pmm_alloc_page();
                if(addresses[i] == NULL) return NULL;
            }
            for(size_t i = 0; i < n; i++) {
                pmm_free_page(old_addresses[i]);
            }
        }
    }
}

void pmm_free(void *addr, size_t n)
{
    for(size_t i = 0; i < n; i++) {
        pmm_free_page(addr + (i * sizeof(void *)));
    }
}
