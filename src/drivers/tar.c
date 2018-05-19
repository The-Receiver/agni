#include <pmm.h>
#include <tar.h>
#include <klib.h>

size_t archive_size;
uint32_t *tar_buffer;

/* Courtesy of OSDev wiki */
uint32_t octal_atoi(const char *in)
{
    uint32_t sz = 0, j = 0, count = 1;
    for(j = 11; j > 0; j--, count += 8) {
        sz += (in[j - 1] - '0') * count;
    }
    return sz;
}

void tar_mount(uint32_t *buf)
{
    tar_buffer = buf;
    for(size_t i = 0; ; i++) {
        tarhdr_t *hdr = (tarhdr_t *) *buf;
        if(hdr->filename[0] == '\0') {
            archive_size = i;
            return;
        }
        uint32_t size = octal_atoi(hdr->size);
        
        buf += size + 512;
        
        if(size % 512) {
            buf += 512;
        }
        
    }
}

tarFILE *tar_open(char *filename)
{
    uint32_t *tarbuf_local = tar_buffer;
    tarFILE *f = (tarFILE *)pmm_alloc_page();
    for(size_t i = 0; i < archive_size; tarbuf_local++, i++) {
        if(kstrcmp((char *)*tarbuf_local, filename) == 0) {
            tarhdr_t *hdr = (tarhdr_t *)*tarbuf_local;
            f->size = octal_atoi(hdr->size);
            f->offset = (void *) 0;
            f->archive_offset = tarbuf_local;
            return f;
        }
    }
    return NULL;
}

void tar_read(void *buf, tarFILE *f, size_t n)
{
    char *buf_ptr = (char *) buf;
    char *filebuf_ptr = (char *) (*(f->archive_offset) + 512);
    kmemcpy(buf_ptr, filebuf_ptr, n);
}

void tar_close(tarFILE *f)
{
    pmm_free_page(f);
}


