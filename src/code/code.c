#include "ps2syscalls.h"
#include "ps2elf.h"
#include "ps2int.h"
#include "ps2cstd.h"
#include "ps2iop.h"
#include "ps2rpc.h"

typedef int (*readBufferInternal_t)(char *, int, int, void *, int, int);
readBufferInternal_t readBufferInternal;

static void setup_pointers() {
    u32 *video_ts_ifo_300e = (u32 *)0x009091a0;
    u32 *video_ts_ifo_300u = (u32 *)0x009090a0;
    u32 *video_ts_ifo_300j = (u32 *)0x00684920;
    if (video_ts_ifo_300e[0] == 0x45444956) {
        readBufferInternal = (readBufferInternal_t)0x00244438;
        sceSifSyncIop = (sceSifSyncIop_t)0x00283460;
        sceSifResetIop = (sceSifResetIop_t)0x002832f8;
        sceSifInitRpc = (sceSifInitRpc_t)0x00207c60;
        sceSifExitRpc = (sceSifExitRpc_t)0x00207e00;
    } else if (video_ts_ifo_300u[0] == 0x45444956) {
        readBufferInternal = (readBufferInternal_t)0x00244378;
        sceSifSyncIop = (sceSifSyncIop_t)0x00283340;
        sceSifResetIop = (sceSifResetIop_t)0x002831d8;
        sceSifInitRpc = (sceSifInitRpc_t)0x00207c60;
        sceSifExitRpc = (sceSifExitRpc_t)0x00207e00;
    } else if (video_ts_ifo_300j[0] == 0x45444956) {
        readBufferInternal = (readBufferInternal_t)0x00244018;
        sceSifSyncIop = (sceSifSyncIop_t)0x002834f0;
        sceSifResetIop = (sceSifResetIop_t)0x00283388;
        sceSifInitRpc = (sceSifInitRpc_t)0x00207c60;
        sceSifExitRpc = (sceSifExitRpc_t)0x00207e00;
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
    int off = (396 - 278) << 11;
    Elf32_Ehdr ehdr;
    readDiscData(off, (u8 *)&ehdr, sizeof(Elf32_Ehdr));

    // Validate ELF header
    if (ehdr.e_phnum > 256 || ehdr.e_phnum == 0) {
        return;  // Invalid program header count
    }

    for (int i = 0; i < ehdr.e_phnum; i++) {
        Elf32_Phdr phdr;
        int phdr_off = off + ehdr.e_phoff + (i * sizeof(Elf32_Phdr));
        readDiscData(phdr_off, (u8 *)&phdr, sizeof(Elf32_Phdr));
        if (phdr.p_type == PT_LOAD) {
            // Validate segment sizes to prevent overflow
            if (phdr.p_memsz > 0x2000000 || phdr.p_filesz > phdr.p_memsz) {
                continue;  // Skip invalid segments
            }
            readDiscData(off + phdr.p_offset, (u8 *)(unsigned long)phdr.p_vaddr, phdr.p_filesz);
            if(phdr.p_memsz > phdr.p_filesz) {
                memset((u8 *)(unsigned long)phdr.p_vaddr + phdr.p_filesz, 0, phdr.p_memsz - phdr.p_filesz);
            }
        }
    }
    FlushCache(0);
    FlushCache(2);
    sceSifResetIop("rom0:UDNL rom0:EELOADCNF", 0);
    while(!sceSifSyncIop());
    sceSifInitRpc(0);
    sceSifExitRpc();
    ExecPS2((void *)(unsigned long)ehdr.e_entry, 0, 0, NULL);
}

__attribute__((section(".text.boot")))
void _start(void) {
    setup_pointers();
    main();
}
