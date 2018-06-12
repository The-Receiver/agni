#include <pmm.h>
#include <klib.h>

#define PAGE_SIZE 0x1000
#define KRNL_BASE 0x1000000

static volatile uint8_t bitmap[(1024 * 1024) / 8]
    __attribute__ ((aligned(PAGE_SIZE)));
    
static int bitmap_read(size_t i)
{
    size_t which = i / 8;
    size_t bit = i % 8;
    
    return (int)((bitmap[which] >> bit) & 1);
}

static void bitmap_write(size_t i, int val)
{
    size_t which = i / 8;
    size_t bit = i % 8;
    
    if (val) {
        bitmap[which] |= (1 << bit);
    } else {
        bitmap[which] &= ~(1 << bit);
    }
}

void pmm_init()
{
	for (size_t i = 0; i < (1024 * 1024) / 8; i++) {
		bitmap[i] = 0;
	}

	bitmap[0] = 1;
}

void *pmm_alloc_page()
{
	for (size_t i = 0; i < (1024 * 1024) / 8; i++) {
        if (!bitmap_read(i)) {
            bitmap_write(i, 1);
            return (void *)(KRNL_BASE + (i * PAGE_SIZE));
        }
	}
	return NULL;
}

void *pmm_alloc(size_t n)
{
    if (n == 1) {
        return pmm_alloc_page();
    }
    size_t i, j, start = 0;
    for (i = 0; i < (1024 * 1024) / 8; i++) {
        if (!bitmap_read(i)) {
            j++;
        } else {
            j = 0;
        }
        if (j == n)
            goto found;
    }
    return NULL;
    
found:
    start = (i - n) + 1;
    for (i = start; i < start + n; i++) {
        bitmap_write(i, 1);
    }
    return (void *)(KRNL_BASE + (start * PAGE_SIZE));
}

void *pmm_realloc(void *ptr, size_t n)
{
    size_t i, j, start = 0;
    size_t bit = (uintptr_t)ptr / PAGE_SIZE;
    
    for(i = bit; i < (n + bit); i++) {
        if (!bitmap_read(i))
            j++;
        else 
            j = 0;
        if(j == n) 
            goto found;
    }
    return NULL;
    
found:
    start = (i - n) + 1;
    for (i = start; i < (start + n); i++) {
        bitmap_write(i, 1);
    }
    return (void *)KRNL_BASE + (start * PAGE_SIZE);
}

void pmm_free_page(void *addr)
{
	size_t bit = (uintptr_t) addr / PAGE_SIZE;
	for (size_t i = 0; i < (1024 * 1024) / 8; i++) {
		for (size_t j = 0; j < 8; j++) {
			if (((i * 8 + j) * PAGE_SIZE) == bit) {
				bitmap[i] &= ~(1 << j);
				return;
			}
		}
	}
}

void pmm_free(void *ptr, size_t n)
{
    size_t bit = ((size_t)(ptr - KRNL_BASE) / PAGE_SIZE);
    
    for (size_t i = bit; i < (bit + n); i++) {
        bitmap_write(i, 0);
    }
}
