#include <klib.h>

#define NORMAL 0
#define FORMAT 1

uint8_t hex_to_ascii[] = {'0', '1', '2','3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f'};

size_t kstrlen(const char *s)
{
    size_t i = 0;
    for(; *s; s++, i++);
    return i;
}

char* kstrchr(char *s, char c)
{
    for(; *s != c; s++);
    return s;
}

char *kstrrchr(char *s, char c)
{
    char *ptr = s + kstrlen(s);
    for(; *ptr; ptr--);
    return ptr;
}

char *kstrstr(char *haystack, char *needle)
{
    size_t j = 0;
    size_t attempts = 0;
    size_t needle_len = kstrlen(needle);
    for(; attempts < 6000; haystack++, attempts++) {
        if(*haystack == *needle) {
            for(size_t i = 0; i < needle_len; i++) {
                if(*(haystack + i) == needle[i]) {
                    j++;
                }
                if(j == needle_len) return haystack;
            }
        }
    }
}

size_t kstrcmp(char *a, char *b)
{
    size_t same;
    size_t l1 = kstrlen(a);
    for(size_t i = 0; i < l1; i++) {
        if(b[i] != a[i]) same++;
    }
    return same;
}

void *kmemcpy(void *dest, void *src, size_t n)
{
    char *dest_m = (char *)dest;
    char *src_m = (char *)src;
    for(size_t i = 0; i < n; i++) {
        dest_m[i] = src_m[i];
    }
    return dest;
}

void *kmemset(void *dest, int c, size_t n)
{
    char *dest_m = (char *) dest;
    for(size_t i = 0; i < n; i++) {
        dest_m[i] = c;
    }
    return dest;
}

void kputhex(uint64_t x)
{
    int i;
    char buf[17] = {0};
    
    if(!x) {
        terminal_puts("0x0");
        return;
    }
    
    for(i = 15; x; i--) {
        buf[i] = hex_to_ascii[x % 16];
        x /= 16;
    }
    
    i++;
    terminal_puts("0x");
    terminal_puts(buf + i);
}

void kputchar(char c)
{
    terminal_putchar(c);
}

void kputs(char *s)
{
    terminal_puts(s);
}

size_t kprintf(const char *fmt, ...)
{
    va_list parameters;
    va_start(parameters, fmt);
	
	char str[50];

    size_t i = 0;
    uint8_t mode = NORMAL;

    while(fmt[i]) {
        switch(mode) {
            case NORMAL:
                switch(fmt[i]) {
                    case '%':
                        mode = FORMAT;
                    break;
                    default:
                        terminal_putchar(fmt[i]);
                    break;
                }
            break;
            case FORMAT:
                switch(fmt[i]) {
                    case 'c':
                        kputchar(va_arg(parameters, int));
                    break;
                    case 's':
                        kputs(va_arg(parameters, char *));
                    break;
                    case 'x':
                        kputhex(va_arg(parameters, uint32_t));
                    break;
                    default:
                        mode = NORMAL;
                    break;
                }
            break;
        }
        i++;
    }
    
    va_end(parameters);
    return i;
} 


