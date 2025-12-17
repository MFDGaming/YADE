#ifndef YADE_PS2MISC
#define YADE_PS2MISC

#include "ps2int.h"

typedef int (*sceSifSyncIop_t)(void);
typedef int (*sceSifResetIop_t)(const char *, int);

extern sceSifSyncIop_t sceSifSyncIop;
extern sceSifResetIop_t sceSifResetIop;

#endif