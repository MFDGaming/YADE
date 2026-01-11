#ifndef YADE_CDVD
#define YADE_CDVD

#include "ps2int.h"

enum CD_NCMD_CMDS {
    CD_NCMD_READ = 0x01,
    CD_NCMD_CDDAREAD,
    CD_NCMD_DVDREAD,
    CD_NCMD_GETTOC,
    CD_NCMD_SEEK,
    CD_NCMD_STANDBY,
    CD_NCMD_STOP,
    CD_NCMD_PAUSE,
    CD_NCMD_STREAM,
    CD_NCMD_CDDASTREAM,
    CD_NCMD_READ_KEY,
    CD_NCMD_NCMD,
    CD_NCMD_READIOPMEM,
    CD_NCMD_DISKREADY,
    CD_NCMD_READCHAIN
};

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

enum SCECdvdInterruptCode {
    CdlNoIntr = 0x00,
    CdlDataReady,
    SCECdComplete,
    CdlAcknowledge,
    CdlDataEnd,
    CdlDiskError,
    SCECdNotReady
};

typedef struct {
    u8 trycount;
    u8 spindlctrl;
    u8 datapattern;
    u8 pad;
} sceCdRMode;

typedef int (*sceCdNCmdDiskReady_t)(void);
typedef int (*_sceCd_ncmd_prechk_t)(int);
typedef void (*_sceCd_cd_read_intr_t)(void *);
typedef int (*sceCdSync_t)(int);
typedef int (*sceCdDiskReady_t)(int);

extern sceCdNCmdDiskReady_t sceCdNCmdDiskReady;
extern _sceCd_ncmd_prechk_t _sceCd_ncmd_prechk;
extern _sceCd_cd_read_intr_t _sceCd_cd_read_intr;
extern sceCdSync_t sceCdSync;
extern sceCdDiskReady_t sceCdDiskReady;

extern int *sceCdCbfunc_num;
extern int *_sceCd_c_cb_sem;
extern void *_sceCd_cd_ncmd;
extern int *_sceCd_ncmd_semid;

int sceCdReadDVDV(u32 lbn, u32 nsectors, void *buf, sceCdRMode *rm);

#endif
