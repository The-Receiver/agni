#include <paging.h>
#include <vmm.h>

#define PAGE_SIZE 0x1000

int map_page(uint32_t *page_directory, void *phys, void *virt, int flags)
{
    uint32_t *page_table;
    size_t pd_index = ((uintptr_t) virt & ((size_t) 0x3ff << 22)) >> 22;
    size_t pt_index = ((uintptr_t) virt & ((size_t) 0x3ff << 12)) >> 12;

    if (page_directory[pd_index] & 0x1) {
        page_table =
            (uint32_t *) (uintptr_t) (page_directory[pd_index] &
                          0xfffff000);
    } else {
        page_table = pmm_alloc(1);
        if (page_table == NULL) {
            return -1;
        }

        for (size_t i = 0; i < 1024; i++) {
            page_table[i] = 0;
        }

        page_directory[pd_index] |= flags;
    }

    page_table[pt_index] = (uintptr_t)phys | flags;
    return 0;
}

int unmap_page(uint32_t *page_directory, void *virt)
{
    size_t pd_index = ((uintptr_t) virt & ((size_t) 0x3ff << 22)) >> 22;
    size_t pt_index = ((uintptr_t) virt & ((size_t) 0x3ff << 12)) >> 12;

    uint32_t *page_table;
    
    if (!(page_directory[pd_index] & 0x1)) {
        return -1;
    }

    page_table = (uint32_t *) page_directory[pd_index];
    page_table[pt_index] = 0;
    return 0;
}

void *vmm_alloc(size_t n, int user)
{
    void *phys = pmm_alloc(n);
    if (phys == NULL) {
        return NULL;
    }
    
    uint32_t *page_directory;
    asm volatile (
        "mov %%cr3, %0"
        : "=a" (page_directory)
    );

    for (size_t i = 0; i < n; i++) {
        void *phys_l = (void *)(uintptr_t) phys + (i * PAGE_SIZE);
        void *virt_l = phys_l + HIGHER_HALF_ADDRESS;
        map_page(page_directory, phys_l, virt_l, (user) ? 0x07 : 0x03);
    }

    return (void *)(uintptr_t) (phys + HIGHER_HALF_ADDRESS);
}

void *vmm_realloc(void *ptr, size_t n, int user)
{
    void *phys = pmm_realloc((ptr - HIGHER_HALF_ADDRESS), n);
    if (phys == NULL) {
        return NULL;
    }
    
    uint32_t *page_directory;
    asm volatile (
        "mov %%cr3, %0"
        : "=a" (page_directory)
    );

    for (size_t i = 0; i < n; i++) {
        void *phys_l = (void *)(uintptr_t) phys + (i * PAGE_SIZE);
        void *virt_l = phys_l + HIGHER_HALF_ADDRESS;
        map_page(page_directory, phys_l, virt_l, (user) ? 0x07 : 0x03);
    }

    return (void *)(uintptr_t) (phys + HIGHER_HALF_ADDRESS);
}

void vmm_free(void *ptr, size_t n)
{
    uint32_t *page_directory;
    asm volatile (
        "mov %%cr3, %0"
        : "=a" (page_directory)
    );
    for (size_t i = 0; i < n; i++) {
        unmap_page(page_directory, (void *)(uintptr_t) ptr + (i * PAGE_SIZE));
        pmm_free((void *)(uintptr_t) (ptr + (i * PAGE_SIZE)) -
             HIGHER_HALF_ADDRESS, 1);
    }
}
