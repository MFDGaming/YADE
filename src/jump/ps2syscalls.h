#ifndef YADE_PS2SYSCALLS
#define YADE_PS2SYSCALLS

void FlushCache(int mode);
void DeleteThread(int thread_id);
void TerminateThread(int thread_id);
void ChangeThreadPriority(int thread_id, int priority);
int GetThreadId(void);
int CancelWakeupThread(int thread_id);
int Exit(int status);

#endif
