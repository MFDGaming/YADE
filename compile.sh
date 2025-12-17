cd "$(dirname "$0")"

rm -rf build
mkdir build

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
    src/jump/jump.S

mipsel-none-elf-objcopy \
    -O binary \
    -j .text \
    build/jump.elf \
    build/jump.bin

gcc src/injector/*.c -o build/injector.elf

cp --recursive fs build/

./build/injector.elf

truncate -s 6KiB build/code.bin
cp build/code.bin build/fs/VIDEO_TS/VIDEO_TS.BUP

genisoimage -dvd-video -V "" -o build/exploit.iso build/fs/
