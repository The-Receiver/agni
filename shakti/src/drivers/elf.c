#include <elf.h>
#include <vfs.h>
#include <vmm.h>
#include <pmm.h>
#include <initrd.h>

elf_exec_t elf_exec(char *path) 
{
    int handle = vfs_open(path, 0);
    if (handle == -1) { return (elf_exec_t){1, 0, 0}; }
    initrd_stat_t *statbuf = vmm_alloc(1, 0);
    if (statbuf == NULL) { return (elf_exec_t){2, 0, 0}; }
    vfs_fstat(handle, statbuf);
    size_t ogsize = statbuf->size;
    while (statbuf->size % 0x1000) statbuf->size++;
    uint8_t *file = vmm_alloc(statbuf->size / 0x1000, 1);
    if (file == NULL) { return (elf_exec_t){3, 0, 0}; }
    vfs_read(handle, file, ogsize);
    vmm_free(statbuf, 1);
    
    elf_fileheader_t *header = (elf_fileheader_t *)file;
    if (header->magic[0] != 0x7f
    &&  header->magic[1] != 'E'
    &&  header->magic[2] != 'L'
    &&  header->magic[3] != 'F') return (elf_exec_t){4, 0, 0};
    if (header->bits != 1) return (elf_exec_t){5, 0, 0};
    if (header->abi != ABI_SYSV) return (elf_exec_t){6, 0, 0};
    if (header->machine != ARCH_I386) return (elf_exec_t){7, 0, 0};
    
    uint32_t *pd = pmm_alloc(1);
    
    if (pd == NULL) return (elf_exec_t){9, 0, 0}; /* BIG BIG PROBLEM if it's null */
    elf_programheader_t *program_header_table = (elf_programheader_t *)((char *)header + header->phoff); 
    
    for (size_t i = 0; i < header->phnum /* hardcoded for now */; i++) {
        kmemcpy((void *)program_header_table[i].paddr, (void *)((char *)header + program_header_table[i].offset), program_header_table[i].filesz);
        int status = map_page(pd, (void *)program_header_table[i].paddr, (void *)program_header_table[i].vaddr, 0x07 /* User page */);
        if (status == -1) return (elf_exec_t) {10, 0, 0};
    }
    return (elf_exec_t){0, (uint32_t)header->entry, (uint32_t)pd};
}
