MAGIC equ 0x1BADB002
FLAGS equ (1<<0) | (1<<1) | (1<<2)
CHECK equ -(MAGIC + FLAGS)
HADDR equ 0
LADDR equ 0
LEADR equ 0
BEADR equ 0
ENTRY equ 0
MTYPE equ 0
%ifdef RES_800_600
RESLX equ 800
RESLY equ 600
%endif
%ifdef RES_1024_768
RESLX equ 1024 
RESLY equ 768
%endif
%ifdef RES_1280_720
RESLX equ 1280
RESLY equ 720
%endif
%ifdef RES_1920_1080
RESLX equ 1920 
RESLY equ 1080 
%endif
DEPTH equ 32

section .multiboot
    dd MAGIC
    dd FLAGS
    dd CHECK
    dd HADDR
    dd LADDR
    dd LEADR
    dd BEADR
    dd ENTRY
    dd MTYPE
    dd RESLX
    dd RESLY
    dd DEPTH
