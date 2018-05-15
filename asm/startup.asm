bits 32
global start
extern kmain
extern paging_init
extern set_up_page_tables

%define LOAD_ADDRESS 0x00100000
%define HIGHER_HALF_ADDRESS 0xC0100000

section .bss
stack_bottom:
    resb 4096
stack_top:

section .data
gdt32:
.null: dq 0
.code:
    dw 0xffff
    dw 0
    db 0
    db 0x9a
    db 11001111b
    db 0
.data:
    dw 0xffff
    dw 0
    db 0
    db 0x92
    db 11001111b
    db 0
.pointer:
    dw $ - gdt32 - 1
    dd gdt32 - HIGHER_HALF_ADDRESS + LOAD_ADDRESS

section .text
start:
    lgdt[gdt32.pointer - HIGHER_HALF_ADDRESS + LOAD_ADDRESS]
    jmp 0x08:(.kernel - HIGHER_HALF_ADDRESS + LOAD_ADDRESS)
.kernel:
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    mov esp, stack_top - HIGHER_HALF_ADDRESS + LOAD_ADDRESS
    
    call set_up_page_tables
    call paging_init
    
    jmp 0x08:.higher_half
    
 .higher_half:   
    sub ebx, HIGHER_HALF_ADDRESS
    add ebx, LOAD_ADDRESS
    push ebx
    call kmain
    
    jmp $
