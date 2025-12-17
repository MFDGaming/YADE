.set noreorder
.section .text.startup

.equ readBufferInternal, 0x00244438
.equ code, (0x2000000 - (0x800 * 2))

.global _start
_start:
	la $a0, 0
	la $a1, 0
	la $a2, 3
	la $a3, code
	la $t0, 1
	la $v0, readBufferInternal
	jalr $v0
	la $t1, 0

run:
    la $a0, 0
	la $v1, 0x64
    syscall 0x64

    la $a0, 2
	la $v1, 0x64
    syscall 0x64

	lui $sp, 0x7000
	la $v0, code
	jr $v0
	ori $sp, 0x4000