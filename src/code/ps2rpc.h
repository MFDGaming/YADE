#ifndef YADE_PS2RPC
#define YADE_PS2RPC

typedef void (*sceSifInitRpc_t)(int);
typedef void (*sceSifExitRpc_t)(void);

extern sceSifInitRpc_t sceSifInitRpc;
extern sceSifExitRpc_t sceSifExitRpc;

#endif
