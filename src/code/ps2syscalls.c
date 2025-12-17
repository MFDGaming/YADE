#include "ps2syscalls.h"

asm (
    ".global ExecPS2 \n"
    "ExecPS2: \n"
    "li $v1, 0x07 \n"
    "syscall 0x07"
);

asm (
    ".global FlushCache \n"
    "FlushCache: \n"
    "li $v1, 0x64 \n"
    "syscall 0x64 \n"
    "jr $ra"
);

asm (
    ".global DeleteThread \n"
    "DeleteThread: \n"
    "li $v1, 0x21 \n"
    "syscall 0x21 \n"
    "jr $ra"
);

asm (
    ".global TerminateThread \n"
    "TerminateThread: \n"
    "li $v1, 0x25 \n"
    "syscall 0x25 \n"
    "jr $ra"
);

asm (
    ".global ChangeThreadPriority \n"
    "ChangeThreadPriority: \n"
    "li $v1, 0x29 \n"
    "syscall 0x29 \n"
    "jr $ra"
);

asm (
    ".global GetThreadId \n"
    "GetThreadId: \n"
    "li $v1, 0x2f \n"
    "syscall 0x2f \n"
    "jr $ra"
);

asm (
    ".global CancelWakeupThread \n"
    "CancelWakeupThread: \n"
    "li $v1, 0x35 \n"
    "syscall 0x35 \n"
    "jr $ra"
);