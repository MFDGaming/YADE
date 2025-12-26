#include "ps2cstd.h"

void *memcpy(void *dest, const void *src, unsigned int n) {
    unsigned char *d = dest;
    const unsigned char *s = src;
    while (n--) {
        *d++ = *s++;
    }
    return dest;
}

void *memmove(void *dest, const void *src, unsigned int n) {
    unsigned char *d = dest;
    const unsigned char *s = src;
    if (d == s || n == 0) {
        return dest;
    }
    if (d < s) {
        for (unsigned int i = 0; i < n; i++) {
            d[i] = s[i];
        }
    } else {
        for (unsigned int i = n; i != 0; i--) {
            d[i - 1] = s[i - 1];
        }
    }
    return dest;
}

void *memset(void *dest, int c, unsigned int n) {
    unsigned char *d = dest;
    while (n--) {
        *d++ = c;
    }
    return dest;
}
