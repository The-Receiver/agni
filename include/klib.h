#ifndef KLIB_H
#define KLIB_H

#include <stdarg.h>
#include <terminal.h>

size_t kstrlen(const char *);
size_t kprintf(const char *, ...);
char *kstrchr(char *, char);
char *kstrrchr(char *, char);
size_t kstrcmp(char *, char *);
char *kstrstr(char *, char *);
void *kmemcpy(void *, void *, size_t);

#endif
