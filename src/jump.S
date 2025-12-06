.section .text.startup

.equ readBufferInternal, 0x00244438
.equ code, 0x1fff800

.global _start
_start:
la $a0, read_code # entry
li $a1, 0x00915df0 # gp
li $a2, 0 # argc
li $a3, 0 # argv
li $v1, 0x07 # ExecPS2
syscall 0x07

read_code:
la $a0, empty_str # name
li $a1, 0 # type
li $a2, 491 # start_sector
li $a3, code # buffer
li $t0, 1 # sector_count
li $v0, readBufferInternal
jalr $v0
li $t1, 0 #unknown_param

clear_cache:
li $a0, 0 # mode
li $v1, 0x64 # FlushCache
syscall 0x64

li $a0, 2 # mode
li $v1, 0x64 # FlushCache
syscall 0x64

run_code:
li $sp, 0x70004000 # stack_top
li $v0, code
j $v0

empty_str:
nop
