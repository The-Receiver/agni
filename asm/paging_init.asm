global paging_init
extern pd

%define LOAD_ADDRESS 0x00100000
%define HIGHER_HALF_ADDRESS 0xC0100000

paging_init:
    mov eax, pd
    sub eax, HIGHER_HALF_ADDRESS
    add eax, LOAD_ADDRESS
    mov cr3, eax
    mov eax, cr0
    or eax, 1<<31
    mov cr0, eax
    ret
