cd "$(dirname "$0")"

dvd_ver="300E"

if [[ "$1" == "3.00j" || "$1" == "3.00J" ]]; then
    echo "Building for DVD Player v3.00J"
    dvd_ver="300J"
elif [[ "$1" == "3.00u" || "$1" == "3.00U" ]]; then
    echo "Building for DVD Player v3.00U"
    dvd_ver="300U"
elif [[ "$1" == "3.02e" || "$1" == "3.02E" ]]; then
    echo "Building for DVD Player v3.02E"
    dvd_ver="302E"
elif [[ "$1" == "3.02c" || "$1" == "3.02C" ]]; then
    echo "Building for DVD Player v3.02C"
    dvd_ver="302C"
elif [[ "$1" == "3.02d" || "$1" == "3.02D" ]]; then
    echo "Building for DVD Player v3.02D"
    dvd_ver="302D"
elif [[ "$1" == "3.02g" || "$1" == "3.02G" ]]; then
    echo "Building for DVD Player v3.02G"
    dvd_ver="302G"
elif [[ "$1" == "3.02j" || "$1" == "3.02J" ]]; then
    echo "Building for DVD Player v3.02J"
    dvd_ver="302J"
elif [[ "$1" == "3.02k" || "$1" == "3.02K" ]]; then
    echo "Building for DVD Player v3.02K"
    dvd_ver="302K"
else
    echo "Building for DVD Player v3.00E/A"
fi

rm -rf build
mkdir build
rm -f fs/VIDEO_TS/VTS_01_0.IFO
rm -f fs/VIDEO_TS/VTS_02_0.BUP
rm -f fs/VIDEO_TS/VTS_02_0.IFO
rm -f fs/VIDEO_TS/VTS_03_0.BUP
rm -f fs/VIDEO_TS/VTS_03_0.IFO
rm -f fs/VIDEO_TS/VTS_04_0.BUP
rm -f fs/VIDEO_TS/VTS_04_0.IFO
cp fs/VIDEO_TS/VTS_01_0.BUP fs/VIDEO_TS/VTS_01_0.IFO
cp fs/VIDEO_TS/VTS_01_0.BUP fs/VIDEO_TS/VTS_02_0.BUP
cp fs/VIDEO_TS/VTS_01_0.BUP fs/VIDEO_TS/VTS_02_0.IFO
cp fs/VIDEO_TS/VTS_01_0.BUP fs/VIDEO_TS/VTS_03_0.BUP
cp fs/VIDEO_TS/VTS_01_0.BUP fs/VIDEO_TS/VTS_03_0.IFO
cp fs/VIDEO_TS/VTS_01_0.BUP fs/VIDEO_TS/VTS_04_0.BUP
cp fs/VIDEO_TS/VTS_01_0.BUP fs/VIDEO_TS/VTS_04_0.IFO

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

truncate -s 8192 build/code.bin
cp build/code.bin build/fs/VIDEO_TS/VIDEO_TS.BUP
cp build/fs/VIDEO_TS/VTS_01_1.VOB build/fs/VIDEO_TS/VTS_02_1.VOB
cp build/fs/VIDEO_TS/VTS_01_1.VOB build/fs/VIDEO_TS/VTS_03_1.VOB
cp build/fs/VIDEO_TS/VTS_01_1.VOB build/fs/VIDEO_TS/VTS_04_1.VOB

genisoimage -dvd-video -V "" -o build/exploit.iso build/fs/
rm fs/VIDEO_TS/VTS_01_0.IFO
rm fs/VIDEO_TS/VTS_02_0.BUP
rm fs/VIDEO_TS/VTS_02_0.IFO
rm fs/VIDEO_TS/VTS_03_0.BUP
rm fs/VIDEO_TS/VTS_03_0.IFO
rm fs/VIDEO_TS/VTS_04_0.BUP
rm fs/VIDEO_TS/VTS_04_0.IFO
