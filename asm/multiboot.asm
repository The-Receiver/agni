MAGIC equ 0x1BADB002
FLAGS equ 0x00000000
CHECK equ -(MAGIC + FLAGS)

section .multiboot
header:
    dd MAGIC
    dd FLAGS
    dd CHECK
