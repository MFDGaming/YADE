#include "ps2cdvd.h"
#include "ps2rpc.h"
#include "ps2cstd.h"
#include "ps2syscalls.h"

sceCdNCmdDiskReady_t sceCdNCmdDiskReady;
_sceCd_ncmd_prechk_t _sceCd_ncmd_prechk;
_sceCd_cd_read_intr_t _sceCd_cd_read_intr;
sceCdSync_t sceCdSync;
sceCdDiskReady_t sceCdDiskReady;

int *sceCdCbfunc_num;
int *_sceCd_c_cb_sem;
void *_sceCd_cd_ncmd;
int *_sceCd_ncmd_semid;

static u32 readData[6] __attribute__((aligned(64)));
static u32 _sceCd_rd_intr_data[36] __attribute__((aligned(64)));

int sceCdReadDVDV(u32 lbn, u32 nsectors, void *buf, sceCdRMode *rm) {
    if (sceCdNCmdDiskReady() == SCECdNotReady) {
        return 0;
    }
    if (_sceCd_ncmd_prechk(CD_NCMD_DVDREAD) == 0) {
        return 0;
    }

    readData[0] = lbn;
    readData[1] = nsectors;
    readData[2] = (u32)(unsigned long)buf;
    readData[3] = (rm->trycount) | (rm->spindlctrl << 8) | (rm->datapattern << 16);
    readData[4] = (u32)(unsigned long)_sceCd_rd_intr_data;

    sceSifWriteBackDCache(buf, nsectors * 2064);
    sceSifWriteBackDCache(_sceCd_rd_intr_data, sizeof(_sceCd_rd_intr_data));
    sceSifWriteBackDCache(readData, sizeof(readData));

    *sceCdCbfunc_num = CD_NCMD_DVDREAD;
    *_sceCd_c_cb_sem = 1;

    if (sceSifCallRpc(_sceCd_cd_ncmd, CD_NCMD_DVDREAD, 1, readData, sizeof(readData), NULL, 0, _sceCd_cd_read_intr, _sceCd_rd_intr_data) < 0) {
        *sceCdCbfunc_num = 0;
        *_sceCd_c_cb_sem = 0;
        SignalSema(*_sceCd_ncmd_semid);
        return 0;
    }

    return 1;
}
