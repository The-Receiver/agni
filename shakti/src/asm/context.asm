global context_switch

; parameters
; eax - page dir
; ebx - regs_t * (context to switch to)
; ecx - pointer to a part of the process table (to fill in the old context)

section .data
pd: dd 0    
    
section .text
context_switch:
    mov [pd], eax
    
    
    
    ret
