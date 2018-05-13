global paging_init
extern pd

paging_init:
    mov eax, pd
    mov cr3, eax
    mov eax, cr0
    or eax, 1<<31
    mov cr0, eax
    ret
