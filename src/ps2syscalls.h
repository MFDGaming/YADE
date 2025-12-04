#ifndef YADE_PS2SYSCALLS
#define YADE_PS2SYSCALLS

void ExecPS2(void *entry, void *gp, int argc, char **argv);
void SetMemoryMode(int mode);
void _InitTLB(void);
void FlushCache(int mode);
void SignalSema(int sema_id);

#endif