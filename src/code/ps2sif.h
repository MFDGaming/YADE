#ifndef YADE_PS2SIF
#define YADE_PS2SIF

#define SIF_INIT (1 << 0)
#define SIF_INIT2 (1 << 1)
#define SIF_CMD_FLG (1 << 17)

#define SIF_DBG_1 (1 << 2)
#define SIF_DBG_2 (1 << 3)
#define SIF_DBG_3 (1 << 4)

#define SIF_FROM_IO	0x00
#define SIF_TO_IO 0x01
#define SIF_FROM_IOP 0x00
#define SIF_TO_IOP 0x01
#define SIF_FROM_EE	0x00
#define SIF_TO_EE 0x01

#define SIF_DMA_INT	0x02
#define SIF_DMA_INT_I 0x02
#define SIF_DMA_INT_O 0x04

#define SIF_DMA_SPR	0x08

#define SIF_DMA_BSN	0x10
#define SIF_DMA_TAG	0x20

#define SIF_DMA_ERT	0x40

#define SIF_MSCOM 0x01
#define SIF_SMCOM 0x02
#define SIF_MSFLG 0x03
#define SIF_SMFLG 0x04

#define SIF_CMDI_SYSTEM 0x80000000
#define SIF_CMDC_CHANGE_SADDR (SIF_CMDI_SYSTEM | 0x00000000)
#define SIF_CMDC_SET_SREG (SIF_CMDI_SYSTEM | 0x00000001)
#define SIF_CMDC_INIT_CMD (SIF_CMDI_SYSTEM | 0x00000002)
#define SIF_CMDC_RESET_CMD (SIF_CMDI_SYSTEM | 0x00000003)

typedef struct {
	unsigned int psize:8;
	unsigned int dsize:24;
	unsigned int daddr;
  	unsigned int fcode;
	unsigned int opt;
} sceSifCmdHdr;

typedef struct {
	sceSifCmdHdr chdr;
	int size;
	int flag;
	char arg[80];
} sceSifCmdResetData;

typedef void (*sceSifInitRpc_t)(int);
typedef void (*sceSifExitRpc_t)(void);

extern sceSifInitRpc_t sceSifInitRpc;
extern sceSifExitRpc_t sceSifExitRpc;

void sceSifWriteBackDCache(void *buffer, int len);
int sceSifSyncIop(void);
int sceSifResetIop(const char *arg, int mode);

#endif
