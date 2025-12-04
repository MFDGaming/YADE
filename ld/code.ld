ENTRY(_start)

SECTIONS
{
    .text 0x1fff800 : {
        *(.text.boot)
        *(.text*)
    }
    .rodata : { *(.rodata*) }
    .data : { *(.data*) }
    .bss : { *(.bss*) *(COMMON) }
}
