#include "ps2syscalls.h"
#include "ps2elf.h"
#include "ps2int.h"
#include "ps2cstd.h"
#include "ps2iop.h"
#include "ps2rpc.h"
#include "ps2cdvd.h"

int no_reset = 0;

static void setup_pointers() {
    u32 *video_ts_ifo_300e = (u32 *)0x009091a0;
    u32 *video_ts_ifo_300u = (u32 *)0x009090a0;
    u32 *video_ts_ifo_300j = (u32 *)0x00684920;
    u32 *video_ts_ifo_302e = (u32 *)0x0090c310;
    u32 *video_ts_ifo_302c = (u32 *)0x006ee290;
    u32 *video_ts_ifo_302d = (u32 *)0x00678410;
    u32 *video_ts_ifo_302g = (u32 *)0x00683d90;
    if (video_ts_ifo_300e[0] == 0x45444956) {
        sceSifSyncIop = (sceSifSyncIop_t)0x00283460;
        sceSifResetIop = (sceSifResetIop_t)0x002832f8;
        sceSifInitRpc = (sceSifInitRpc_t)0x00207c60;
        sceSifExitRpc = (sceSifExitRpc_t)0x00207e00;
        sceSifCallRpc = (sceSifCallRpc_t)0x002084a0;
        sceSifWriteBackDCache = (sceSifWriteBackDCache_t)0x00209fc0;
        sceCdNCmdDiskReady = (sceCdNCmdDiskReady_t)0x00268a58;
        _sceCd_ncmd_prechk = (_sceCd_ncmd_prechk_t)0x002688e8;
        _sceCd_cd_read_intr = (_sceCd_cd_read_intr_t)0x002682ac;
        sceCdSync = (sceCdSync_t)0x00268af0;
        sceCdDiskReady = (sceCdDiskReady_t)0x00268ff0;
        sceCdCbfunc_num = (int *)0x008c9114;
        _sceCd_c_cb_sem = (int *)0x008c90f0;
        _sceCd_cd_ncmd = (void *)0x008ca290;
        _sceCd_ncmd_semid = (int *)0x008c90e8;
    } else if (video_ts_ifo_300u[0] == 0x45444956) {
        sceSifSyncIop = (sceSifSyncIop_t)0x00283340;
        sceSifResetIop = (sceSifResetIop_t)0x002831d8;
        sceSifInitRpc = (sceSifInitRpc_t)0x00207c60;
        sceSifExitRpc = (sceSifExitRpc_t)0x00207e00;
        sceSifCallRpc = (sceSifCallRpc_t)0x002084a0;
        sceSifWriteBackDCache = (sceSifWriteBackDCache_t)0x00209fc0;
        sceCdNCmdDiskReady = (sceCdNCmdDiskReady_t)0x00268938;
        _sceCd_ncmd_prechk = (_sceCd_ncmd_prechk_t)0x002687c8;
        _sceCd_cd_read_intr = (_sceCd_cd_read_intr_t)0x00268190;
        sceCdSync = (sceCdSync_t)0x002689d0;
        sceCdDiskReady = (sceCdDiskReady_t)0x00268ed0;
        sceCdCbfunc_num = (int *)0x008c9014;
        _sceCd_c_cb_sem = (int *)0x008c8ff0;
        _sceCd_cd_ncmd = (void *)0x008ca190;
        _sceCd_ncmd_semid = (int *)0x008c8fe8;
        no_reset = 1; // For some reason 3.00U breaks if the IOP is reset
    } else if (video_ts_ifo_300j[0] == 0x45444956) {
        sceSifSyncIop = (sceSifSyncIop_t)0x002834f0;
        sceSifResetIop = (sceSifResetIop_t)0x00283388;
        sceSifInitRpc = (sceSifInitRpc_t)0x00207c60;
        sceSifExitRpc = (sceSifExitRpc_t)0x00207e00;
        sceSifCallRpc = (sceSifCallRpc_t)0x002084a0;
        sceSifWriteBackDCache = (sceSifWriteBackDCache_t)0x00209fc0;
        sceCdNCmdDiskReady = (sceCdNCmdDiskReady_t)0x00268ae0;
        _sceCd_ncmd_prechk = (_sceCd_ncmd_prechk_t)0x00268970;
        _sceCd_cd_read_intr = (_sceCd_cd_read_intr_t)0x00268338;
        sceCdSync = (sceCdSync_t)0x00268b78;
        sceCdDiskReady = (sceCdDiskReady_t)0x00269078;
        sceCdCbfunc_num = (int *)0x00644894;
        _sceCd_c_cb_sem = (int *)0x00644870;
        _sceCd_cd_ncmd = (void *)0x00645a10;
        _sceCd_ncmd_semid = (int *)0x00644868;
    } else if (video_ts_ifo_302e[0] == 0x45444956) {
        sceSifSyncIop = (sceSifSyncIop_t)0x00284d00;
        sceSifResetIop = (sceSifResetIop_t)0x00284b98;
        sceSifInitRpc = (sceSifInitRpc_t)0x00207ce0;
        sceSifExitRpc = (sceSifExitRpc_t)0x00207e80;
        sceSifCallRpc = (sceSifCallRpc_t)0x00208520;
        sceSifWriteBackDCache = (sceSifWriteBackDCache_t)0x0020a040;
        sceCdNCmdDiskReady = (sceCdNCmdDiskReady_t)0x00258b00;
        _sceCd_ncmd_prechk = (_sceCd_ncmd_prechk_t)0x00258990;
        _sceCd_cd_read_intr = (_sceCd_cd_read_intr_t)0x00258358;
        sceCdSync = (sceCdSync_t)0x00258b98;
        sceCdDiskReady = (sceCdDiskReady_t)0x00259098;
        sceCdCbfunc_num = (int *)0x004d4254;
        _sceCd_c_cb_sem = (int *)0x004d4230;
        _sceCd_cd_ncmd = (void *)0x004d53d0;
        _sceCd_ncmd_semid = (int *)0x004d4228;
    } else if (video_ts_ifo_302c[0] == 0x45444956) {
        sceSifSyncIop = (sceSifSyncIop_t)0x00284f30;
        sceSifResetIop = (sceSifResetIop_t)0x00284dc8;
        sceSifInitRpc = (sceSifInitRpc_t)0x00207ce0;
        sceSifExitRpc = (sceSifExitRpc_t)0x00207e80;
        sceSifCallRpc = (sceSifCallRpc_t)0x00208520;
        sceSifWriteBackDCache = (sceSifWriteBackDCache_t)0x0020a040;
        sceCdNCmdDiskReady = (sceCdNCmdDiskReady_t)0x00258ae0;
        _sceCd_ncmd_prechk = (_sceCd_ncmd_prechk_t)0x00258970;
        _sceCd_cd_read_intr = (_sceCd_cd_read_intr_t)0x00258338;
        sceCdSync = (sceCdSync_t)0x00258b78;
        sceCdDiskReady = (sceCdDiskReady_t)0x00259078;
        sceCdCbfunc_num = (int *)0x004d4454;
        _sceCd_c_cb_sem = (int *)0x004d4430;
        _sceCd_cd_ncmd = (void *)0x004d55d0;
        _sceCd_ncmd_semid = (int *)0x004d4428;
    } else if (video_ts_ifo_302d[0] == 0x45444956) {
        sceSifSyncIop = (sceSifSyncIop_t)0x00284d90;
        sceSifResetIop = (sceSifResetIop_t)0x00284c28;
        sceSifInitRpc = (sceSifInitRpc_t)0x00207ce0;
        sceSifExitRpc = (sceSifExitRpc_t)0x00207e80;
        sceSifCallRpc = (sceSifCallRpc_t)0x00208520;
        sceSifWriteBackDCache = (sceSifWriteBackDCache_t)0x0020a040;
        sceCdNCmdDiskReady = (sceCdNCmdDiskReady_t)0x00258ae0;
        _sceCd_ncmd_prechk = (_sceCd_ncmd_prechk_t)0x00258970;
        _sceCd_cd_read_intr = (_sceCd_cd_read_intr_t)0x00258338;
        sceCdSync = (sceCdSync_t)0x00258b78;
        sceCdDiskReady = (sceCdDiskReady_t)0x00259078;
        sceCdCbfunc_num = (int *)0x004d42d4;
        _sceCd_c_cb_sem = (int *)0x004d42b0;
        _sceCd_cd_ncmd = (void *)0x004d5450;
        _sceCd_ncmd_semid = (int *)0x004d42a8;
    } else if (video_ts_ifo_302g[0] == 0x45444956) {
        sceSifSyncIop = (sceSifSyncIop_t)0x00284ec0;
        sceSifResetIop = (sceSifResetIop_t)0x00284d58;
        sceSifInitRpc = (sceSifInitRpc_t)0x00207ce0;
        sceSifExitRpc = (sceSifExitRpc_t)0x00207e80;
        sceSifCallRpc = (sceSifCallRpc_t)0x00208520;
        sceSifWriteBackDCache = (sceSifWriteBackDCache_t)0x0020a040;
        sceCdNCmdDiskReady = (sceCdNCmdDiskReady_t)0x00258ae0;
        _sceCd_ncmd_prechk = (_sceCd_ncmd_prechk_t)0x00258970;
        _sceCd_cd_read_intr = (_sceCd_cd_read_intr_t)0x00258338;
        sceCdSync = (sceCdSync_t)0x00258b78;
        sceCdDiskReady = (sceCdDiskReady_t)0x00259078;
        sceCdCbfunc_num = (int *)0x004d43d4;
        _sceCd_c_cb_sem = (int *)0x004d43b0;
        _sceCd_cd_ncmd = (void *)0x004d5550;
        _sceCd_ncmd_semid = (int *)0x004d43a8;
    }
}

int readSector(int n, u8 *s) {
    const int max_tries = 30;
    int tries = 0;
    sceCdRMode rm;
    rm.spindlctrl = SCECdSpinNom;
    rm.datapattern = SCECdSecS2048;
    rm.trycount = max_tries;

    while (tries < max_tries) {
		sceCdDiskReady(0);
		if (sceCdReadDVDV(n, 1, s, &rm)) {
			break;
		}
		++tries;
	}
    if (tries == max_tries) {
		return -1;
	}
    sceCdSync(0);
    return 0;
}

static void readDiscData(int off, u8 *dest, int len) {
    u8 tmp[2064];
    if (len <= 0) {
        return;
    }
    int skip = off & 0x7FF;
    int s = off >> 11;

    if (skip) {
        int first = 0x800 - skip;
        if (first > len) first = len;
        readSector(s++, tmp);
        memcpy(dest, &tmp[12+skip], first);
        dest += first;
        len -= first;
    }
    int sc = len >> 11;
    for (int i = 0; i < sc; ++i) {
        readSector(s++, tmp);
        memcpy(dest, &tmp[12], 0x800);
        dest += 0x800;
    }
    int rem = len - (sc << 11);
    if (rem > 0) {
        readSector(s, tmp);
        memcpy(dest, &tmp[12], rem);
    }
}

void main() {
    int off = 516 << 11;
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
    sceSifInitRpc(0);
    sceSifExitRpc();
    if (!no_reset) {
        sceSifResetIop("rom0:UDNL rom0:EELOADCNF", 0);
        while(!sceSifSyncIop());
    }
    ExecPS2((void *)(unsigned long)ehdr.e_entry, 0, 0, NULL);
}

__attribute__((section(".text.boot")))
void _start(void) {
    setup_pointers();
    main();
}
