#ifndef KLIB_H
#define KLIB_H

#include <stdarg.h>
#include <terminal.h>

void kputs(char *);
void kputchar(char);
size_t kstrlen(const char *);
size_t kprintf(const char *, ...);
char *kstrchr(char *, char);
char *kstrrchr(char *, char);
size_t kstrcmp(char *, char *);
char *kstrstr(char *, char *);
void *kmemcpy(void *, void *, size_t);
void *kmemset(void *, int, size_t);

typedef struct {
    uint32_t gs, fs, es, ds;
    uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax;
    uint32_t int_no, err_code;
    uint32_t eip, cs, eflags, useresp, ss;
} regs_t;


static inline void outb(uint16_t port, uint8_t val)
{
    asm volatile (
        "outb %1, %0"
        :
        : "a"(val), "Nd"(port)
    );
}

static inline uint8_t inb(uint16_t port)
{
    uint8_t ret;
    asm volatile (
        "inb %0, %1"
        : "=a"(ret)
        : "Nd"(port)
    );
    return ret;
}

#endif
