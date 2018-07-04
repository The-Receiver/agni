#include <elf.h>
#include <vfs.h>
#include <vmm.h>
#include <pmm.h>
#include <initrd.h>

elf_exec_t *elf_exec(char *path) 
{
    elf_exec_t *ret = vmm_alloc(1, 0);
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
    
    elf_fileheader_t *header = (elf_fileheader_t *)file;
    if (header->magic[0] != 0x7f
    &&  header->magic[1] != 'E'
    &&  header->magic[2] != 'L'
    &&  header->magic[3] != 'F') return NULL;
    if (header->bits != 1) return NULL;
    if (header->abi != ABI_SYSV) return NULL;
    if (header->machine != ARCH_I386) return NULL;
    
    uint32_t *pd = pmm_alloc(1);
    
    if (pd == NULL) return NULL; /* BIG BIG PROBLEM if it's null */
    elf_programheader_t *program_header_table = (elf_programheader_t *)((char *)header + header->phoff); 
    
    for (size_t i = 0; i < header->phnum /* hardcoded for now */; i++) {
        kmemcpy((void *)program_header_table[i].paddr, (void *)((char *)header + program_header_table[i].offset), program_header_table[i].filesz);
        int status = map_page(pd, (void *)program_header_table[i].paddr, (void *)program_header_table[i].vaddr, 0x07 /* User page */);
        if (status == -1) return NULL;
    }
    ret->entry = (uint32_t)header->entry;
    ret->page_directory = (uint32_t)pd;
    return ret;
}
