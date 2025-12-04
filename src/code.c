#include "ps2syscalls.h"
#include "ps2elf.h"
#include "ps2int.h"
#include "ps2cdvd.h"
#include "ps2cstd.h"
#include "ps2misc.h"

int readSector(int n, u8 *s) {
    sceCdRMode rm;
    rm.spindlctrl = SCECdSpinStm;
    rm.datapattern = SCECdSecS2048;
    rm.trycount = 0xff;
    sceCdReadDVDV(n, 1, s, &rm);
    sceCdSync(0);
}

int readDiscData(int off, u8 *dest, int len) {
    u8 tmp[2064]; // ECC: 0-11, SECTOR: 12-2059, PAD: 2060-2063
    int skip = off % 0x800;
    int rem = (len - skip) % 0x800;
    int sc = (len - (skip + rem)) >> 11;
    int s = off >> 11;
    if (skip > 0) {
        int r = 0x800 - skip;
        if (len < r) {
            r = len;
        }
        readSector(s++, tmp);
        memcpy(dest, &tmp[12+skip], r);
        dest += r;
    }
    for (int i = 0; i < sc; ++i) {
        readSector(s++, tmp);
        memcpy(dest, &tmp[12], 0x800);
        dest += 0x800;
    }
    if (rem > 0) {
        readSector(s, tmp);
        memcpy(dest, &tmp[12], rem);
    }
}

void main() {
    int off = 1242 << 11;
    sceCdInit(SCECdINIT);
    sceCdDiskReady(0);
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
    sceCdInit(SCECdEXIT);
    FlushCache(0);
    FlushCache(2);
    while (!sceSifRebootIop("rom0:EELOADCNF"));
    while(!sceSifSyncIop());
    ExecPS2((void *)ehdr.e_entry, 0, 0, NULL);
}

__attribute__((section(".text.boot")))
void _start(void) {
    main();
    for(;;);
}
