#include "ps2syscalls.h"

asm (
    ".global ExecPS2 \n"
    "ExecPS2: \n"
    "li $v1, 0x07 \n"
    "syscall 0x07"
);
asm (
    ".global SetMemoryMode \n"
    "SetMemoryMode: \n"
    "li $v1, 0x85 \n"
    "syscall 0x85 \n"
    "jr $ra"
);
asm (
    ".global _InitTLB \n"
    "_InitTLB: \n"
    "li $v1, 0x82 \n"
    "syscall 0x82 \n"
    "jr $ra"
);
asm (
    ".global FlushCache \n"
    "FlushCache: \n"
    "li $v1, 0x64 \n"
    "syscall 0x64 \n"
    "jr $ra"
);
asm (
    ".global SignalSema \n"
    "SignalSema: \n"
    "li $v1, 0x42 \n"
    "syscall 0x42 \n"
    "jr $ra"
);