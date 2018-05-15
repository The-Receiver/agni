ENTRY(start)                /* the name of the entry label */

SECTIONS {
    . = 0xc0100000;          /* the code should be loaded at 1 MB */
    
    .multiboot ALIGN(0x1000) : AT(ADDR(.multiboot) - 0xc0100000 + 0x0100000)
    {
        *(.multiboot)
    }

    .text ALIGN (0x1000) : AT(ADDR(.text) - 0xc0100000 + 0x0100000)  /* align at 4 KB */
    {
        *(.text)             /* all text sections from all files */
    }

    .rodata ALIGN (0x1000) : AT(ADDR(.rodata) - 0xc0100000 + 0x0100000)/* align at 4 KB */
    {
        *(.rodata*)          /* all read-only data sections from all files */
    }

    .data ALIGN (0x1000) : AT(ADDR(.data) - 0xc0100000 + 0x0100000)  /* align at 4 KB */
    {
        *(.data)             /* all data sections from all files */
    }
    
    .bss ALIGN (0x1000) : AT(ADDR(.bss) - 0xc0100000 + 0x0100000)
    {
        *(.bss)
    }
}
