#ifndef YADE_PS2IOP
#define YADE_PS2IOP

typedef int (*sceSifSyncIop_t)(void);
typedef int (*sceSifResetIop_t)(const char *, int);
typedef int (*sceSifRebootIop_t)(const char *);
typedef void *(*sceSifAllocIopHeap_t)(int);
typedef int (*sceSifFreeIopHeap_t)(void *);
typedef int (*sceSifInitIopHeap_t)(void);

extern sceSifSyncIop_t sceSifSyncIop;
extern sceSifResetIop_t sceSifResetIop;
extern sceSifRebootIop_t sceSifRebootIop;
extern sceSifAllocIopHeap_t sceSifAllocIopHeap;
extern sceSifFreeIopHeap_t sceSifFreeIopHeap;
extern sceSifInitIopHeap_t sceSifInitIopHeap;

#endif