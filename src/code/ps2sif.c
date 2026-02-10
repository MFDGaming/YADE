#include "ps2sif.h"
#include "ps2syscalls.h"

sceSifInitRpc_t sceSifInitRpc;
sceSifExitRpc_t sceSifExitRpc;

void sceSifWriteBackDCache(void *buffer, int len) {
    if (len > 0) {
        unsigned long saddr = (unsigned long)buffer & ~63;
        unsigned long eaddr = ((unsigned long)buffer + (len - 1)) & ~63;
        unsigned long chunk64_cnt = ((eaddr - saddr) >> 6) + 1;
        unsigned long chunk8_rem = chunk64_cnt & 7;
        unsigned long chunk8_cnt = chunk64_cnt >> 3;
        while (chunk8_rem--) {
            asm volatile ("sync" ::: "memory");
            asm volatile ("cache 0x18, 0(%0)" :: "r"(saddr) : "memory");
            asm volatile ("sync" ::: "memory");
            saddr += 64;
        }
        while (chunk8_cnt--) {
            asm volatile ("sync" ::: "memory");
            asm volatile ("cache 0x18, 0(%0)" :: "r"(saddr) : "memory");
            asm volatile ("sync" ::: "memory");
            asm volatile ("cache 0x18, 64(%0)" :: "r"(saddr) : "memory");
            asm volatile ("sync" ::: "memory");
            asm volatile ("cache 0x18, 128(%0)" :: "r"(saddr) : "memory");
            asm volatile ("sync" ::: "memory");
            asm volatile ("cache 0x18, 192(%0)" :: "r"(saddr) : "memory");
            asm volatile ("sync" ::: "memory");
            asm volatile ("cache 0x18, 256(%0)" :: "r"(saddr) : "memory");
            asm volatile ("sync" ::: "memory");
            asm volatile ("cache 0x18, 320(%0)" :: "r"(saddr) : "memory");
            asm volatile ("sync" ::: "memory");
            asm volatile ("cache 0x18, 384(%0)" :: "r"(saddr) : "memory");
            asm volatile ("sync" ::: "memory");
            asm volatile ("cache 0x18, 448(%0)" :: "r"(saddr) : "memory");
            asm volatile ("sync" ::: "memory");
            saddr += 512;
        }
    }
}

int sceSifSyncIop(void) {
    return (sceSifGetReg(SIF_SMFLG) & 0x40000) != 0;
}

int sceSifResetIop(const char *arg, int mode) {
    static sceSifCmdResetData rst_data __attribute__((aligned(64)));
    sceSifDmaData dmat;
    int arglen = 0;
    
    sceSifStopDma();
    if (*arg != '\0') {
        for (;;) {
            rst_data.arg[arglen] = arg[arglen];
            if (arg[++arglen] == '\0') break;
        }
    }
    rst_data.chdr.psize = sizeof(rst_data);
    rst_data.chdr.fcode = SIF_CMDC_RESET_CMD;
    rst_data.flag = mode;
    rst_data.size = arglen;

    dmat.data = (unsigned int)(unsigned long)(&rst_data);
    dmat.addr = sceSifGetReg(SIF_CMDI_SYSTEM);
    dmat.size = sizeof(rst_data);
    dmat.mode = SIF_DMA_ERT | SIF_DMA_INT_O;

    sceSifWriteBackDCache(&rst_data, sizeof(rst_data));
    sceSifSetReg(SIF_SMFLG, 0x40000);
    if (!sceSifSetDma(&dmat, 1)) {
        return 0;
    }
    sceSifSetReg(SIF_SMFLG, 0x10000);
    sceSifSetReg(SIF_SMFLG, 0x20000);
    sceSifSetReg(SIF_CMDC_INIT_CMD, 0);
    sceSifSetReg(SIF_CMDI_SYSTEM, 0);
    return 1;
}
