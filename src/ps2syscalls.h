#ifndef YADE_PS2SYSCALLS
#define YADE_PS2SYSCALLS

typedef struct t_SifDmaTransfer {
    void *src, *dest;
    int size;
    int attr;
} SifDmaTransfer_t;

void ExecPS2(void *entry, void *gp, int argc, char **argv);
void SetMemoryMode(int mode);
void _InitTLB(void);
void FlushCache(int mode);
void SignalSema(int sema_id);
int sceSifSetDma(SifDmaTransfer_t *dmat, int count);
int sceSifDmaStat(int trid);

#endif