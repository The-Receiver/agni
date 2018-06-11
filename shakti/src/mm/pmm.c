#include <pmm.h>
#include <klib.h>

#define PAGE_SIZE 0x1000
#define KRNL_BASE 0x1000000

volatile uint8_t bitmap[(1024 * 1024) / 8]
    __attribute__ ((aligned(PAGE_SIZE)));
    
static int bitmap_read(size_t i)
{
    size_t which = i / 8;
    size_t bit = i % 8;
    
    return (int) (bitmap[which] >> bit) & 1;
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
		for (size_t j = 0; j < CHAR_BIT; j++) {
			if (!((bitmap[i] >> j) & 1)) {
				bitmap[i] |= (1 << j);
				return (void *)(KRNL_BASE +
						((i * 8) + j) * PAGE_SIZE);
			}
		}
	}
	return NULL;
}

void *pmm_alloc(size_t n)
{
    size_t i = 0;
    size_t j = 0;
    
    for (i = 0; i < (1024 * 1024) / 8; i++) {
        if (!bitmap_read(i))
            j++;
        else
            j = 0;
        if (j == n) {
            size_t start = i - n - 1;
            
            for (size_t j = start; j < n; j++) {
                bitmap_write(j, 1);
            }
            
            return (void *)(start * PAGE_SIZE);
        }
    }
    return NULL;
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
    for (size_t i = (uintptr_t)ptr/PAGE_SIZE; i < n + (uintptr_t)ptr/PAGE_SIZE; i++) {
        bitmap_write(i, 0);
    }
}
