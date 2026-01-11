#ifndef YADE_PS2RPC
#define YADE_PS2RPC

typedef void (*sceSifInitRpc_t)(int);
typedef void (*sceSifExitRpc_t)(void);
typedef int (*sceSifCallRpc_t)(void *, int, int, void *, int, void *, int, void (*)(void *), void *);
typedef void (*sceSifWriteBackDCache_t)(void *, int); 

extern sceSifInitRpc_t sceSifInitRpc;
extern sceSifExitRpc_t sceSifExitRpc;
extern sceSifCallRpc_t sceSifCallRpc;
extern sceSifWriteBackDCache_t sceSifWriteBackDCache;

#endif
