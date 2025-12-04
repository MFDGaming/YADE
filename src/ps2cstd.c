#include "ps2cstd.h"

void *memcpy(void *dest, void *src, unsigned int n) {
    int i;
    if (!n) {
        return NULL;
    }
    for (i = 0; i < n; ++i) {
        ((unsigned char *)dest)[i] = ((unsigned char *)src)[i];
    }
    return dest;
}

void *memset(void *dest, int c, unsigned int n) {
    int i;
    if (!n) {
        return NULL;
    }
    for (i = 0; i < n; ++i) {
        ((unsigned char *)dest)[i] = c;
    }
	return dest;
}