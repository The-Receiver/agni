#include <klib.h>

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
