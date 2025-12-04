#ifndef YADE_PS2CDVD
#define YADE_PS2CDVD

#include "ps2int.h"

#define SCECdNotReady 0x06
#define CD_NCMD_DVDREAD 0x03
#define SIF_RPC_M_NOWAIT 0x01

enum SCECdvdSectorType {
    SCECdSecS2048 = 0,
    SCECdSecS2328,
    SCECdSecS2340,
    SCECdSecS2352 = 0,
    SCECdSecS2368,
    SCECdSecS2448,
};

enum SCECdvdSpinValue {
    SCECdSpinMax = 0,
    SCECdSpinStm = 0,
    SCECdSpinDvdDL0 = 0,
    SCECdSpinNom = 1,
    SCECdSpinX1,
    SCECdSpinX2,
    SCECdSpinX4,
    SCECdSpinX12,
    SCECdSpinNm2 = 10,
    SCECdSpin1p6,
    SCECdSpinMx = 20,
};

enum SCECdvdMModeMediaType {
    SCECdMmodeCd = 1,
    SCECdMmodeDvd
};

enum SCECdvdInitMode {
    SCECdINIT = 0x00,
    SCECdINoD,
    SCECdEXIT = 0x05
};

typedef struct {
    u8 trycount;
    u8 spindlctrl;
    u8 datapattern;
    u8 pad;
} sceCdRMode;

typedef struct {
    u32 lsn;
    u32 size;
    char name[16];
    u8 date[8];
} sceCdlFILE;

typedef s32 (*sceCdInit_t)(s32);
typedef int (*sceCdMmode_t)(int);
typedef int (*sceCdInitEeCB_t)(int, void *, int);
typedef int (*sceCdDiskReady_t)(int);
typedef int (*sceCdSearchFile_t)(sceCdlFILE *, const char *);
typedef void (*_sceCd_cd_read_intr_t)(void *);
typedef int (*sceCdNCmdDiskReady_t)(void);
typedef int (*_CdCheckNCmd_t)(int);
typedef int (*sceCdSync_t)(int);

extern sceCdInit_t sceCdInit;
extern sceCdMmode_t sceCdMmode;
extern sceCdInitEeCB_t sceCdInitEeCB;
extern sceCdDiskReady_t sceCdDiskReady;
extern sceCdSearchFile_t sceCdSearchFile;
extern _sceCd_cd_read_intr_t _sceCd_cd_read_intr;
extern sceCdNCmdDiskReady_t sceCdNCmdDiskReady;
extern _CdCheckNCmd_t _CdCheckNCmd;
extern sceCdSync_t sceCdSync;

int sceCdReadDVDV(u32 lbn, u32 nsectors, void *buf, sceCdRMode *rm);

#endif