#include "ps2syscalls.h"

typedef int (*readBufferInternal_t)(char *, int, int, void *, int, int);
readBufferInternal_t readBufferInternal = (readBufferInternal_t)0x00244438;

typedef void (*code_t)(void);
code_t code = (code_t)((void *)(0x2000000 - (0x800 * 2)));

static void killAllThreads(void) {
    int tid = GetThreadId();
    ChangeThreadPriority(tid, 0);
    CancelWakeupThread(tid);
    for (int i = 1; i < 256; ++i) {
        if(i != tid) {
            TerminateThread(i);
            DeleteThread(i);
        }
    }
}

__attribute__((section(".text.boot")))
void _start(void) {
    asm ("la $sp, 0x70004000");
    killAllThreads();
    readBufferInternal("", 0, 3, code, 1, 0);
    FlushCache(0);
    FlushCache(2);
    code();
    for (;;);
}
