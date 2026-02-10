#ifndef YADE_PS2SYSCALLS
#define YADE_PS2SYSCALLS

typedef struct {
    unsigned int data;
	unsigned int addr;
	unsigned int size;
	unsigned int mode;
} sceSifDmaData;

void ExecPS2(void *entry, void *gp, int argc, char **argv);
void FlushCache(int mode);
void DeleteThread(int thread_id);
void TerminateThread(int thread_id);
void ChangeThreadPriority(int thread_id, int priority);
int GetThreadId(void);
int CancelWakeupThread(int thread_id);
int Exit(int status);
void sceSifStopDma(void);
int sceSifGetReg(unsigned int register_num);
int sceSifSetReg(unsigned int register_num, int register_value);
unsigned int sceSifSetDma(sceSifDmaData *sdd, int len);

#endif
