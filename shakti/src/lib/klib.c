#include <klib.h>
#include <pit.h>

#define NORMAL 0
#define FORMAT 1

int kpow(int base, int exp)
{
    int result = 1;
    for (;;) {
        if (exp & 1) {
            result *= base;
        }
        exp >>= 1;
        if (!exp)
            break;
        base *= base;
    }
    return result;
}

size_t kstrlen(const char *s)
{
    size_t i = 0;
    for (; *s; s++, i++) ;
    return i;
}

char *kstrchr(char *s, char c)
{
    for (; *s != c; s++) ;
    return s;
}

char *kstrrchr(char *s, char c)
{
    char *ptr = s + kstrlen(s);
    for (; *ptr != c; ptr--) ;
    return ptr;
}

char *kstrstr(char *haystack, char *needle)
{
    size_t j = 0;
    size_t attempts = 0;
    size_t needle_len = kstrlen(needle);
    for (; attempts < 6000; haystack++, attempts++) {
        if (*haystack == *needle) {
            for (size_t i = 0; i < needle_len; i++) {
                if (*(haystack + i) == needle[i]) {
                    j++;
                }
                if (j == needle_len)
                    return haystack;
            }
        }
    }
    return NULL;
}

int kstrcmp(char *a, char *b)
{
    while (*(a) && (*(a) == *(b))) {
        a++;
        b++;
    }
    return *(const unsigned char *)a - *(const unsigned char *)b;
}

void *kmemcpy(void *dest, void *src, size_t n)
{
    char *dest_m = (char *)dest;
    char *src_m = (char *)src;
    for (size_t i = 0; i < n; i++) {
        dest_m[i] = src_m[i];
    }
    return dest;
}

void *kmemset(void *dest, int c, size_t n)
{
    char *dest_m = (char *)dest;
    for (size_t i = 0; i < n; i++) {
        dest_m[i] = c;
    }
    return dest;
}

uint8_t hex_to_ascii[] = { '0', '1', '2', '3', '4', '5', '6', '7',
    '8', '9', 'a', 'b', 'c', 'd', 'e', 'f'
};

uint8_t dec_to_ascii[] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9' };

void kputhex(uint64_t x)
{
    int i;
    char buf[17] = { 0 };

    if (!x) {
        terminal_puts("0x0");
        return;
    }

    for (i = 15; x; i--) {
        buf[i] = hex_to_ascii[x % 16];
        x /= 16;
    }

    i++;
    terminal_puts("0x");
    terminal_puts(buf + i);
}

char *kstrrev(char *s)
{
    char *p1, *p2;

    if (!s || !*s)
        return s;

    for (p1 = s, p2 = s + kstrlen(s) - 1; p2 > p1; ++p1, --p2) {
        *p1 ^= *p2;
        *p2 ^= *p1;
        *p1 ^= *p2;
    }

    return s;
}

void kputdec(uint64_t x)
{
    int i = 0;
    char buf[50] = { 0 };

    while (x) {
        buf[i] = dec_to_ascii[x % 10];
        x /= 10;
        i++;
    }

    kputs(kstrrev(buf));
}

int katoi(char *buf)
{
    int n = 0;
    if (kstrcmp(buf, "0") == 0) {
        return 0;
    }
    for (int i = 0; buf[i] != 0; i++) {
        int power = kpow(10, i);
        n += (power * (buf[i] - '0'));
    }
    return n;
}

void kputchar(char c)
{
    terminal_putchar(c);
}

void kputs(char *s)
{
    terminal_puts(s);
}

void kdelay(uint64_t secs)
{
    uint64_t initial = pit_ticks();
    while ((pit_ticks() - initial) != secs) ;
}

size_t kprintf(const char *fmt, ...)
{
    va_list parameters;
    va_start(parameters, fmt);

    size_t i = 0;
    uint8_t mode = NORMAL;

    while (fmt[i]) {
        switch (mode) {
        case NORMAL:
            switch (fmt[i]) {
            case '%':
                mode = FORMAT;
                break;
            default:
                terminal_putchar(fmt[i]);
                break;
            }
            break;
        case FORMAT:
            switch (fmt[i]) {
            case 'c':
                kputchar(va_arg(parameters, int));
                break;
            case 's':
                kputs(va_arg(parameters, char *));
                break;
            case 'x':
                kputhex(va_arg(parameters, uint32_t));
                break;
            case 'u':
                kputdec(va_arg(parameters, uint32_t));
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
