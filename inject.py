import os

home = os.getcwd() + os.sep
build = home + "build" + os.sep
fs = build + "fs" + os.sep

path = build + "code.bin"
data = bytearray(open(path, "rb").read())
ln = len(data)
rem = 0x587ff if (ln < 0x587ff) else (0x587ff - ln)
data += b'\x00' * rem
open(path, "wb").write(data[0:0x587ff])

path = build + "jump.bin"
data = bytearray(open(path, "rb").read())
ln = min(len(data), 0xba)

path = fs + "VIDEO_TS" + os.sep + "VTS_02_0.IFO"
ifo = bytearray(open(path, "rb").read())
ifo[0x25c:0x25c + ln] = data
ifo[0x20e2:0x20e2 + 4] = b"\x07\x0b\x07\x0b"
open(path, "wb").write(ifo)

path = fs + "VIDEO_TS" + os.sep + "VTS_01_1.VOB"
vob = bytearray(open(path, "rb").read())
vob[0x238ae:0x238ae + 2] = b"\x86\x16"
open(path, "wb").write(vob)
