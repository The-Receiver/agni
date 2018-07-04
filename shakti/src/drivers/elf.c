#include <elf.h>
#include <vfs.h>
#include <vmm.h>
#include <pmm.h>
#include <initrd.h>

static uint8_t magic[] = {0x7f, 'E', 'L', 'F'};

elf_exec_t elf_exec(char *path) 
{
    int handle = vfs_open(path, 0);
    if (handle == -1) { return (elf_exec_t){-1, NULL, NULL}; }
    initrd_stat_t *statbuf = vmm_alloc(1, 0);
    if (statbuf == NULL) { return (elf_exec_t){-2, NULL, NULL}; }
    vfs_fstat(handle, statbuf);
    while (statbuf->size % 0x1000) statbuf->size++;
    uint8_t *file = vmm_alloc(statbuf->size / 0x1000, 1);
    if (file == NULL) { return (elf_exec_t){-3, NULL, NULL}; }
    vfs_read(handle, file, statbuf->size / 0x1000);
    vmm_free(statbuf, 1);
    
    /** Now the real parsing begins **/
    /* Step 1: cast the archive to an ELF program header and make sure the data is valid */
    
    elf_fileheader_t *header = (elf_fileheader_t *)file;
    if (kmemcmp(header->magic, magic, 4) != 0) { return (elf_exec_t){-4, NULL, NULL}; }
    if (header->class != 1) { return (elf_exec_t){-5, NULL, NULL}; }
    if (header->endianness != 1) { return (elf_exec_t){-6, NULL, NULL}; }
    if (header->abi != 0x00) { return (elf_exec_t){-7, NULL, NULL};}
    if (header->type != ET_EXEC) {return (elf_exec_t){-8, NULL, NULL}; 
        
    /* Step 2: parse the program headers and construct a virtual address space for the program */
    /* Allocate a page for the pd (it must be physical */
    
    uint32_t *pd = pmm_alloc(1);
    if (pd == NULL) return (elf_exec_t){-9, NULL, NULL} /* BIG BIG PROBLEM if it's null */
    elf_programheader_t *program_header_table = (elf_programheader_t *)((char *)header + header->phoff); 
    
    /* Loop through the program headers and map the appropriate pages to the appropriate addresses */
    for (size_t i = 0; i < header->phnum; i++) {
        if (program_header_table[i].align != 0 && program_header_table[i.align] != 1) {
            if (p_vaddr != (p_offset % p_align)) {
                return (elf_exec_t){-10, NULL, NULL};
            }
        }
        kmemcpy(program_header_table[i].paddr, (void *)((char *)header + program_header_table[i].offset), program_header_table[i].filesz);
        int status = map_page(pd, (void *)program_header_table[i].paddr, (void *)program_header_table[i].vaddr, 0x07 /* User page */);
        if (status == -1) return { (elf_exec_t) {-11 - ((int) i), NULL, NULL}; }
    }
    return (elf_exec_t){0, (void *)header->entry, pd}
}
