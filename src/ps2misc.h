#ifndef YADE_PS2MISC
#define YADE_PS2MISC

#include "ps2int.h"

typedef int (*sceSifSyncIop_t)(void);
typedef int (*sceSifResetIop_t)(const char *, int);
typedef int (*sceSifRebootIop_t)(const char *);
typedef void (*sceSifWriteBackDCache_t)(void *, u32);

extern sceSifSyncIop_t sceSifSyncIop;
extern sceSifResetIop_t sceSifResetIop;
extern sceSifRebootIop_t sceSifRebootIop;
extern sceSifWriteBackDCache_t sceSifWriteBackDCache;

#endif