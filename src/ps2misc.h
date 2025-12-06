#ifndef YADE_PS2MISC
#define YADE_PS2MISC

#include "ps2int.h"

typedef void (*sceSifWriteBackDCache_t)(void *, u32);
typedef int (*sceSifLoadModuleBuffer_t)(void *, int, char *);
typedef int (*_sceSifLoadModuleBuffer_t)(void *, u32, const char *, int *);

extern sceSifWriteBackDCache_t sceSifWriteBackDCache;
extern sceSifLoadModuleBuffer_t sceSifLoadModuleBuffer;
extern _sceSifLoadModuleBuffer_t _sceSifLoadModuleBuffer;

#endif