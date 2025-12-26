#ifndef YADE_PS2SYSCALLS
#define YADE_PS2SYSCALLS

void ExecPS2(void *entry, void *gp, int argc, char **argv);
void FlushCache(int mode);
void DeleteThread(int thread_id);
void TerminateThread(int thread_id);
void ChangeThreadPriority(int thread_id, int priority);
int GetThreadId(void);
int CancelWakeupThread(int thread_id);

#endif
