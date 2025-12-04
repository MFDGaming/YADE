#include "ps2cdvd.h"
#include "ps2syscalls.h"
#include "ps2rpc.h"
#include "ps2cstd.h"
#include "ps2misc.h"

sceCdInit_t sceCdInit = (sceCdInit_t)0x00268d70;
sceCdMmode_t sceCdMmode = (sceCdMmode_t)0x002691e8;
sceCdInitEeCB_t sceCdInitEeCB = (sceCdInitEeCB_t)0x002681d4;
sceCdDiskReady_t sceCdDiskReady = (sceCdDiskReady_t)0x00268ff0;
sceCdSearchFile_t sceCdSearchFile = (sceCdSearchFile_t)0x002688c8;
_sceCd_cd_read_intr_t _sceCd_cd_read_intr = (_sceCd_cd_read_intr_t)0x002682ac;
sceCdNCmdDiskReady_t sceCdNCmdDiskReady = (sceCdNCmdDiskReady_t)0x00268a58;
_CdCheckNCmd_t _CdCheckNCmd = (_CdCheckNCmd_t)0x002688e8;
sceCdSync_t sceCdSync = (sceCdSync_t)0x00268af0;

static void *clientNCmd = (void *)0x008ca290;
static int *CdCallbackNum = (int *)0x008c9114;
static int *cbSema = (int *)0x008c90f0;
static int *nCmdSemaId = (int *)0x008c90e8;

static u32 readData[6] __attribute__((aligned(64)));
static u32 _rd_intr_data[64] __attribute__((aligned(64)));
static u32 curReadPos __attribute__((aligned(64)));

int sceCdReadDVDV(u32 lbn, u32 nsectors, void *buf, sceCdRMode *rm) {
    if (sceCdNCmdDiskReady() == SCECdNotReady) {
        return 0;
    }
    if (_CdCheckNCmd(CD_NCMD_DVDREAD) == 0) {
        return 0;
    }

    readData[0] = lbn;
    readData[1] = nsectors;
    readData[2] = (u32)buf;
    readData[3] = (rm->trycount) | (rm->spindlctrl << 8) | (rm->datapattern << 16);
    readData[4] = (u32)_rd_intr_data;

    sceSifWriteBackDCache(buf, nsectors * 2064);

    *CdCallbackNum = CD_NCMD_DVDREAD;
    *cbSema = 1;

    if (sceSifCallRpc(clientNCmd, CD_NCMD_DVDREAD, SIF_RPC_M_NOWAIT, readData, 24, NULL, 0, _sceCd_cd_read_intr, _rd_intr_data) < 0) {
        *CdCallbackNum = 0;
        *cbSema        = 0;
        SignalSema(*nCmdSemaId);
        return 0;
    }

    return 1;
}