#include "ps2syscalls.h"

#ifdef V300E
#define RBI_ADDR 0x00244438
#elif V300U
#define RBI_ADDR 0x00244378
#elif V300J
#define RBI_ADDR 0x00244018
#elif V302E
#define RBI_ADDR 0x002566d8
#elif V302C
#define RBI_ADDR 0x002566b8
#elif V302D
#define RBI_ADDR 0x002566b8
#endif

typedef int (*readBufferInternal_t)(char *, int, int, void *, int, int);
readBufferInternal_t readBufferInternal = (readBufferInternal_t)RBI_ADDR;

typedef void (*code_t)(void);
code_t code = (code_t)((void *)(0x2000000 - (0x800 * 3)));

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
    readBufferInternal("", 0, 4, code, 2, 0);
    FlushCache(0);
    FlushCache(2);
    code();
    for (;;);
}
