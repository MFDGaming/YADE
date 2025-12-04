cd "$(dirname "$0")"

rm -rf build
mkdir build

mipsel-none-elf-gcc \
    -T ld/code.ld \
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
    src/*.c

mipsel-none-elf-objcopy \
    -O binary \
    -j .text \
    -j .rodata \
    -j .data \
    -j .bss \
    build/code.elf \
    build/code.bin

mipsel-none-elf-gcc \
    -T ld/jump.ld \
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
    src/jump.S

mipsel-none-elf-objcopy \
    -O binary \
    -j .text \
    build/jump.elf \
    build/jump.bin

cp --recursive fs build/

python3 inject.py

cp build/code.bin build/fs/VIDEO_TS/VTS_02_1.VOB

genisoimage -dvd-video -V "" -o build/exploit.iso build/fs
