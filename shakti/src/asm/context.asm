global context_switch

; parameters
; eax - page dir
; ebx - regs_t *

section .data
    pd: dd 0

section .text
context_switch:
    mov [pd], eax
    
    push dword [ebx + 56]
    push dword [ebx + 60]
    push dword [ebx + 64]
    push dword [ebx + 68]
    push dword [ebx + 72]
    
    mov eax, ebx
    
    mov gs, [eax]
    mov fs, [eax + 4]
    mov es, [eax + 8]
    mov ds, [eax + 12]
    
    mov edi, [eax + 16]
    mov esi, [eax + 20]
    mov ebp, [eax + 24]
    mov ebx, [eax + 32]
    mov edx, [eax + 36]
    mov ecx, [eax + 40]
    mov eax, [eax + 44]
    
    push eax
    mov eax, [pd]
    mov cr3, eax
    pop eax
    
    iret
