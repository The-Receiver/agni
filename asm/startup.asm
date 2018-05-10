bits 32
global start
extern kmain

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
    dd gdt32

section .text
start:
    lgdt[gdt32.pointer]
    jmp 0x08:.kernel
.kernel:
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    mov esp, stack_top
    
    mov byte [0xb800], 'a'
    mov byte [0xb8001], 0x0f
    
    push ebx
    call kmain
    
    jmp $
