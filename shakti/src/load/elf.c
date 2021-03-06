#include <elf.h>
#include <vfs.h>
#include <vmm.h>
#include <pmm.h>
#include <initrd.h>

elf_exec_t *elf_exec(char *path) 
{
    /* Some trippy file I/O stuff, ideally should not be this long */
    elf_exec_t *ret = vmm_alloc(1, 0);
    int handle = vfs_open(path, 0);
    if (handle == -1) return NULL;
    initrd_stat_t *statbuf = vmm_alloc(1, 0);
    if (statbuf == NULL) return NULL;
    vfs_fstat(handle, statbuf);
    size_t ogsize = statbuf->size;
    while (statbuf->size % 0x1000) statbuf->size++;         /* Alignment smh */
    uint8_t *file = vmm_alloc(statbuf->size / 0x1000, 1);   /* Allocate that amount */
    if (file == NULL) return NULL;
    vfs_read(handle, file, ogsize);                         /* Read the file into the buffer */
    vmm_free(statbuf, 1);                                   /* Free the stat buffer */
    vfs_close(handle);                                      /* Close the opened file handle */
    
    elf_fileheader_t *header = (elf_fileheader_t *)file;
    if (header->magic[0] != 0x7f
    && (header->magic[1]) != 'E'
    && (header->magic[2]) != 'L'
    && (header->magic[3]) != 'F') return NULL;
    if (header->type != ET_EXEC) return NULL;
    if (header->bits != 1) return NULL;
    if (header->abi != ABI_SYSV) return NULL;
    if (header->machine != ARCH_I386) return NULL;
    
    uint32_t *pd = pmm_alloc(1);                            /* Allocate pages for the */
    uint32_t *pt = pmm_alloc(1);                            /* pt and pd respectively */
    
    if (pd == NULL) return NULL; /* BIG BIG PROBLEM if it's null */
    if (pt == NULL) return NULL; /* BIG BIG PROBLEM here as well */
    
    size_t i = 0;
    for (i = 0; i < 1024 * 1024; i++) {
        pt[i] = (i * 0x1000) | 0x03;
    }
    
    for (i = 0; i < 1024; i++) {
        pd[i] = (uintptr_t) &pt[i * 1024] | 0x03;
    }
    
    for (i = 0; i < 4; i++)
        pd[768 + i] = (uintptr_t) (&pt[i * 1024]) | 0x03;
        
    pmm_free(pt, 1);
    
    elf_programheader_t *program_header_table = (elf_programheader_t *)((char *)header + header->phoff); 
    
    for (i = 0; i < header->phnum; i++) {
        if (program_header_table[i].type != PT_LOAD) {
            continue;
        }
        void *phys;
        size_t n;
        while (program_header_table[i].memsz % 0x1000) program_header_table[i].memsz++;
        
        for (n = 0; n < program_header_table[i].memsz / 0x1000; n++) {
            phys = pmm_alloc(1);
            if (phys == NULL) return NULL;
            int status = map_page(pd,  phys, (void *)program_header_table[i].vaddr + (n * 0x1000), 0x07 /* User page */);
            if (status == -1) return NULL;
        }
        void *init_phys = (void *)(uintptr_t)(phys - (0x1000 * n));
        kmemcpy(init_phys, (char *)header + program_header_table[i].offset, program_header_table[i].filesz);
    }
    
    ret->entry = (uint32_t)header->entry;
    ret->page_directory = (uint32_t)pd;
    return ret;
}

void elf_mod_load(char *path)
{
    int handle = vfs_open(path, 0);
    if (handle == -1) return NULL;
    initrd_stat_t *statbuf = vmm_alloc(1, 0);
    if (statbuf == NULL) return NULL;
    vfs_fstat(handle, statbuf);
    size_t ogsize = statbuf->size;
    while (statbuf->size % 0x1000) statbuf->size++;
    uint8_t *file = vmm_alloc(statbuf->size / 0x1000, 1);
    if (file == NULL) return NULL;
    vfs_read(handle, file, ogsize);
    vmm_free(statbuf, 1);
    vfs_close(handle);
    
    elf_fileheader_t *header = (elf_fileheader_t *)file;
    if (header->magic[0] != 0x7f
    && (header->magic[1]) != 'E'
    && (header->magic[2]) != 'L'
    && (header->magic[3]) != 'F') return NULL;
    if (header->type != ET_DYN) return NULL;
    if (header->bits != 1) return NULL;
    if (header->abi != ABI_SYSV) return NULL;
    if (header->machine != ARCH_I386) return NULL;
    
    size_t i = 0;
    
    elf_programheader_t *program_header_table = (elf_programheader_t *)((char *)header + header->phoff);
    for (i = 0; i < header->phnum; i++) {
        if (program_header_table[i].type != PT_DYNAMIC) {
            continue;
        }
    }
}
