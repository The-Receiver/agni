#include <klib.h>
#include <limits.h>
#include <paging.h>

#define PAGE_SIZE 0x1000

uintptr_t pd[1024] __attribute__ ((aligned(PAGE_SIZE)));
uintptr_t pt[1024 * 1024] __attribute__ ((aligned(PAGE_SIZE)));
;

void set_up_page_tables(void)
{
    uintptr_t *pt_pa =
        (uintptr_t *) ((char *)pt - HIGHER_HALF_ADDRESS + LOAD_ADDRESS);
    uintptr_t *pd_pa =
        (uintptr_t *) ((char *)pd - HIGHER_HALF_ADDRESS + LOAD_ADDRESS);

    for (size_t i = 0; i < 1024 * 1024; i++) {
        pt_pa[i] = (i * PAGE_SIZE) | 0x03;
    }

    for (size_t i = 0; i < 1024; i++) {
        pd_pa[i] = (uintptr_t) & pt_pa[i * 1024] | 0x03;
    }

    for (size_t i = 0; i < 4; i++)
        pd_pa[768 + i] = (uintptr_t) (&pt_pa[i * 1024]) | 0x03;
}
