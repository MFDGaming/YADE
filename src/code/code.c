#include "ps2syscalls.h"
#include "ps2elf.h"
#include "ps2cstd.h"
#include "ps2sif.h"

#ifndef BOOT_FILE_SIZE
#define BOOT_FILE_SIZE 0
#endif
#define BOOT_FILE_SC ((BOOT_FILE_SIZE >> 11) + ((BOOT_FILE_SIZE % 0x800) ? 1 : 0))

int elf_rsect;

typedef int (*readBufferInternal_t)(char *, int, int, void *, int, int);
readBufferInternal_t readBufferInternal;

static void setup_pointers() {
    unsigned int *video_ts_ifo_300e = (unsigned int *)0x009091a0;
    unsigned int *video_ts_ifo_300u = (unsigned int *)0x009090a0;
    unsigned int *video_ts_ifo_300j = (unsigned int *)0x00684920;
    unsigned int *video_ts_ifo_302e = (unsigned int *)0x0090c310;
    unsigned int *video_ts_ifo_302c = (unsigned int *)0x006ee290;
    unsigned int *video_ts_ifo_302d = (unsigned int *)0x00678410;
    unsigned int *video_ts_ifo_302g = (unsigned int *)0x00683d90;
    unsigned int *video_ts_ifo_302j = (unsigned int *)0x00685f10;
    unsigned int *video_ts_ifo_302k = (unsigned int *)0x00682810;
    unsigned int *video_ts_ifo_302u = (unsigned int *)0x0090c210;
    unsigned int *video_ts_ifo_303e = (unsigned int *)0x00923d10;
    unsigned int *video_ts_ifo_303j = (unsigned int *)0x0069de10;
    unsigned int *video_ts_ifo_304m = (unsigned int *)0x0095ac70;
    unsigned int *video_ts_ifo_304j = (unsigned int *)0x006d4df0;

    elf_rsect = 516 - 284;
    if (video_ts_ifo_300e[0] == 0x45444956) {
        sceSifInitRpc = (sceSifInitRpc_t)0x00207c60;
        sceSifExitRpc = (sceSifExitRpc_t)0x00207e00;
        readBufferInternal = (readBufferInternal_t)0x00244438;
        return;
    }
    if (video_ts_ifo_300u[0] == 0x45444956) {
        sceSifInitRpc = (sceSifInitRpc_t)0x00207c60;
        sceSifExitRpc = (sceSifExitRpc_t)0x00207e00;
        readBufferInternal = (readBufferInternal_t)0x00244378;
        return;
    }
    if (video_ts_ifo_300j[0] == 0x45444956) {
        sceSifInitRpc = (sceSifInitRpc_t)0x00207c60;
        sceSifExitRpc = (sceSifExitRpc_t)0x00207e00;
        readBufferInternal = (readBufferInternal_t)0x00244018;
        return;
    }
    if (video_ts_ifo_302e[0] == 0x45444956) {
        sceSifInitRpc = (sceSifInitRpc_t)0x00207ce0;
        sceSifExitRpc = (sceSifExitRpc_t)0x00207e80;
        readBufferInternal = (readBufferInternal_t)0x002566d8;
        return;
    }
    if (video_ts_ifo_302c[0] == 0x45444956) {
        sceSifInitRpc = (sceSifInitRpc_t)0x00207ce0;
        sceSifExitRpc = (sceSifExitRpc_t)0x00207e80;
        readBufferInternal = (readBufferInternal_t)0x002566b8;
        return;
    }
    if (video_ts_ifo_302d[0] == 0x45444956) {
        sceSifInitRpc = (sceSifInitRpc_t)0x00207ce0;
        sceSifExitRpc = (sceSifExitRpc_t)0x00207e80;
        readBufferInternal = (readBufferInternal_t)0x002566b8;
        return;
    }
    if (video_ts_ifo_302g[0] == 0x45444956) {
        sceSifInitRpc = (sceSifInitRpc_t)0x00207ce0;
        sceSifExitRpc = (sceSifExitRpc_t)0x00207e80;
        readBufferInternal = (readBufferInternal_t)0x002566b8;
        return;
    }
    if (video_ts_ifo_302j[0] == 0x45444956) {
        sceSifInitRpc = (sceSifInitRpc_t)0x00207d20;
        sceSifExitRpc = (sceSifExitRpc_t)0x00207ec0;
        readBufferInternal = (readBufferInternal_t)0x00256330;
        return;
    }
    if (video_ts_ifo_302k[0] == 0x45444956) {
        sceSifInitRpc = (sceSifInitRpc_t)0x00207ce0;
        sceSifExitRpc = (sceSifExitRpc_t)0x00207e80;
        readBufferInternal = (readBufferInternal_t)0x002566a8;
        return;
    }
    if (video_ts_ifo_302u[0] == 0x45444956) {
        sceSifInitRpc = (sceSifInitRpc_t)0x00207d20;
        sceSifExitRpc = (sceSifExitRpc_t)0x00207ec0;
        readBufferInternal = (readBufferInternal_t)0x00256668;
        return;
    }
    elf_rsect = 544 - 284;
    if (video_ts_ifo_303e[0] == 0x45444956) {
        sceSifInitRpc = (sceSifInitRpc_t)0x002082a0;
        sceSifExitRpc = (sceSifExitRpc_t)0x00208440;
        readBufferInternal = (readBufferInternal_t)0x00262360;
        return;
    }
    if (video_ts_ifo_303j[0] == 0x45444956) {
        sceSifInitRpc = (sceSifInitRpc_t)0x002082a0;
        sceSifExitRpc = (sceSifExitRpc_t)0x00208440;
        readBufferInternal = (readBufferInternal_t)0x00262340;
        return;
    }
    if (video_ts_ifo_304m[0] == 0x45444956) {
        sceSifInitRpc = (sceSifInitRpc_t)0x00208260;
        sceSifExitRpc = (sceSifExitRpc_t)0x00208400;
        readBufferInternal = (readBufferInternal_t)0x00261548;
        return;
    }
    if (video_ts_ifo_304j[0] == 0x45444956) {
        sceSifInitRpc = (sceSifInitRpc_t)0x002082a0;
        sceSifExitRpc = (sceSifExitRpc_t)0x00208440;
        readBufferInternal = (readBufferInternal_t)0x00261560;
        return;
    }
}

void main() {
    unsigned char *elf_data = (unsigned char *)0x100000;
    Elf32_Ehdr *ehdr = (Elf32_Ehdr *)elf_data;

    sceSifInitRpc(0);
    readBufferInternal("", 0, elf_rsect, elf_data, BOOT_FILE_SC, 0);
    sceSifExitRpc();

    // Validate ELF header
    if (ehdr->e_phnum > 256 || ehdr->e_phnum == 0) {
        Exit(0);  // Invalid program header count
    }

    for (int i = 0; i < ehdr->e_phnum; ++i) {
        int phdr_off = ehdr->e_phoff + (i * sizeof(Elf32_Phdr));
        Elf32_Phdr *phdr = (Elf32_Phdr *)(elf_data + phdr_off);

        if (phdr->p_type == PT_LOAD) {
            // Validate segment sizes to prevent overflow
            if (phdr->p_memsz > 0x2000000 || phdr->p_filesz > phdr->p_memsz) {
                continue;  // Skip invalid segments
            }
            memcpy((unsigned char *)(unsigned long)phdr->p_vaddr, elf_data + phdr->p_offset, phdr->p_filesz);
            if(phdr->p_memsz > phdr->p_filesz) {
                memset((unsigned char *)(unsigned long)phdr->p_vaddr + phdr->p_filesz, 0, phdr->p_memsz - phdr->p_filesz);
            }
        }
    }

    FlushCache(0);
    FlushCache(2);

    while (!sceSifResetIop("", 0));
    while(!sceSifSyncIop());

    ExecPS2((void *)(unsigned long)ehdr->e_entry, 0, 0, NULL);
}

__attribute__((section(".text.boot")))
void _start(void) {
    setup_pointers();
    main();
}
