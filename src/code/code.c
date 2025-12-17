#include "ps2syscalls.h"
#include "ps2elf.h"
#include "ps2int.h"
#include "ps2cstd.h"
#include "ps2misc.h"
#include "ps2rpc.h"

typedef int (*readBufferInternal_t)(char *, int, int, void *, int, int);
readBufferInternal_t readBufferInternal = (readBufferInternal_t)0x00244438;

static void killAllThreads(void) {
    int tid = GetThreadId();
    ChangeThreadPriority(tid, 0);
    CancelWakeupThread(tid);
    for (int i = 1; i < 256; ++i) {
        if(i != tid){
            TerminateThread(i);
            DeleteThread(i);
        }
    }
}

static void readDiscData(int off, u8 *dest, int len) {
    u8 tmp[0x800];
    if (len <= 0) {
        return;
    }
    int skip = off & 0x7FF;
    int s = off >> 11;

    if (skip) {
        int first = 0x800 - skip;
        if (first > len) first = len;
        readBufferInternal("", 0, s++, tmp, 1, 0);
        memcpy(dest, &tmp[skip], first);
        dest += first;
        len -= first;
    }
    int sc = len >> 11;
    for (int i = 0; i < sc; ++i) {
        readBufferInternal("", 0, s++, tmp, 1, 0);
        memcpy(dest, tmp, 0x800);
        dest += 0x800;
    }
    int rem = len - (sc << 11);
    if (rem > 0) {
        readBufferInternal("", 0, s, tmp, 1, 0);
        memcpy(dest, tmp, rem);
    }
}

void main() {
    int off = (391 - 281) << 11;
    Elf32_Ehdr ehdr;
    readDiscData(off, (u8 *)&ehdr, sizeof(Elf32_Ehdr));
    for (int i = 0; i < ehdr.e_phnum; i++) {
        Elf32_Phdr phdr;
        int phdr_off = off + ehdr.e_phoff + (i * sizeof(Elf32_Phdr));
        readDiscData(phdr_off, (u8 *)&phdr, sizeof(Elf32_Phdr));
        if (phdr.p_type == PT_LOAD) {
            readDiscData(off + phdr.p_offset, (u8 *)phdr.p_vaddr, phdr.p_filesz);
            if(phdr.p_memsz > phdr.p_filesz) {
                memset((u8 *)phdr.p_vaddr + phdr.p_filesz, 0, phdr.p_memsz - phdr.p_filesz);
            }
        }
    }
    FlushCache(0);
    FlushCache(2);
    sceSifResetIop("rom0:UDNL rom0:EELOADCNF", 0);
    while(!sceSifSyncIop());
    sceSifInitRpc(0);
    sceSifExitRpc();
    ExecPS2((void *)ehdr.e_entry, 0, 0, NULL);
}

__attribute__((section(".text.boot")))
void _start(void) {
    killAllThreads();
    main();
}
