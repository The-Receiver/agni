#ifndef KLIB_H
#define KLIB_H

#include <stdarg.h>
#include <terminal.h>

int katoi(char *);
void kputs(char *);
void kputchar(char);
void outb(uint16_t port, uint8_t val);
uint8_t inb(uint16_t port);
void kdelay(uint64_t);
size_t kstrlen(const char *);
size_t kprintf(const char *, ...);
char *kstrchr(char *, char);
char *kstrrchr(char *, char);
int kstrcmp(char *, char *);
char *kstrstr(char *, char *);
int kmemcmp(void *, void *, size_t);
void *kmemcpy(void *, void *, size_t);
void *kmemset(void *, int, size_t);

typedef struct {
    uint32_t gs, fs, es, ds;
    uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax;
    uint32_t int_no, err_code;
    uint32_t eip, cs, eflags, useresp, ss;  /* 0, 4, 8, 12, 16, 20, 24, 28, 32, 36, 40, 44, 48, 52, 56, 60, 64,
                           68, 72, 76 */
} regs_t;


#endif
