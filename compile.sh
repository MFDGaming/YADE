cd "$(dirname "$0")"

dvd_ver="300E"

rm -rf build
mkdir build
rm -f fs/VIDEO_TS/VTS_01_0.IFO
rm -f fs/VIDEO_TS/VTS_02_0.BUP
rm -f fs/VIDEO_TS/VTS_02_0.IFO
cp fs/VIDEO_TS/VTS_01_0.BUP fs/VIDEO_TS/VTS_01_0.IFO
cp fs/VIDEO_TS/VTS_01_0.BUP fs/VIDEO_TS/VTS_02_0.BUP
cp fs/VIDEO_TS/VTS_01_0.BUP fs/VIDEO_TS/VTS_02_0.IFO

mipsel-none-elf-gcc \
    -T src/ld/code.ld \
    -march=r5900 \
    -mabi=eabi \
    -mno-gpopt \
    -G0 \
    -nostartfiles \
    -nostdlib \
    -ffreestanding \
    -fno-toplevel-reorder \
    -Os \
    -Wl,-z,max-page-size=0x1 \
    -o build/code.elf \
    src/code/*.c

mipsel-none-elf-objcopy \
    -O binary \
    -j .text \
    -j .rodata \
    -j .data \
    -j .bss \
    build/code.elf \
    build/code.bin

mipsel-none-elf-gcc \
    -DV$dvd_ver \
    -Wl,--defsym=V$dvd_ver=1 \
    -T src/ld/jump.ld \
    -march=r5900 \
    -mabi=eabi \
    -mno-gpopt \
    -G0 \
    -nostartfiles \
    -nostdlib \
    -ffreestanding \
    -fno-toplevel-reorder \
    -Os \
    -Wl,-z,max-page-size=0x1 \
    -o build/jump.elf \
    src/jump/jump.c \
    src/code/ps2syscalls.c \
    -I src/code/

mipsel-none-elf-objcopy \
    -O binary \
    -j .text \
    -j .rodata \
    -j .data \
    -j .bss \
    build/jump.elf \
    build/jump.bin

gcc -DV$dvd_ver src/injector/*.c -o build/injector.elf

cp --recursive fs build/

./build/injector.elf

truncate -s 6144 build/code.bin
cp build/code.bin build/fs/VIDEO_TS/VIDEO_TS.BUP
cp build/fs/VIDEO_TS/VTS_01_1.VOB build/fs/VIDEO_TS/VTS_02_1.VOB

genisoimage -dvd-video -V "" -o build/exploit.iso build/fs/
rm fs/VIDEO_TS/VTS_01_0.IFO
rm fs/VIDEO_TS/VTS_02_0.BUP
rm fs/VIDEO_TS/VTS_02_0.IFO
