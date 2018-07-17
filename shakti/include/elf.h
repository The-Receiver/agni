#ifndef ELF_H
#define ELF_H

#include <klib.h>

#define ET_NONE     0x0000
#define ET_REL      0x0001
#define ET_EXEC     0x0002
#define ET_DYN      0x0003
#define ET_CORE     0x0004
#define ET_LOOS     0xfe00
#define ET_HIOS     0xfeff
#define ET_LOPROC   0xff00
#define ET_HIPROC   0xffff

#define PT_NULL         0x00000000
#define PT_LOAD         0x00000001
#define PT_DYNAMIC      0x00000002
#define PT_INTERP       0x00000003
#define PT_NOTE         0x00000004
#define PT_SHLIB        0x00000005
#define PT_PHDR         0x00000006
#define PT_TLS          0x00000007
#define PT_LOOS         0x60000000
#define PT_HIOS         0x6fffffff
#define PT_LOPROC       0x70000000
#define PT_HIPROC       0x7fffffff

#define ABI_SYSV        0x00
#define ARCH_I386            0x03

/* ELF info header, for the user */
typedef struct {
    uint32_t entry;
    uint32_t phys_entry;
    uint32_t page_directory;
} elf_exec_t; 

/* Main file header */
typedef struct {
    uint8_t magic[4];
    uint8_t bits;
    uint8_t endianness;
    uint8_t version;
    uint8_t abi;
    uint8_t abiversion;
    uint8_t pad[7];
    uint16_t type;
    uint16_t machine;
    uint32_t version2;
    uint32_t entry;
    uint32_t phoff;
    uint32_t shoff;
    uint32_t flags;
    uint16_t ehsize;
    uint16_t phentsize;
    uint16_t phnum;
    uint16_t shentsize;
    uint16_t shnum;
    uint16_t shstrndx;
} elf_fileheader_t;
/* Program header */
typedef struct {
    uint32_t type;
    uint32_t offset;
    uint32_t vaddr;
    uint32_t paddr;
    uint32_t filesz;
    uint32_t memsz;
    uint32_t flags;
    uint32_t align;
} elf_programheader_t;

elf_exec_t *elf_exec(char *);

#endif
