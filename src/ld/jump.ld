ENTRY(_start)

IS_V300E = DEFINED(V300E);
IS_V300U = DEFINED(V300U);
IS_V300J = DEFINED(V300J);

BV300E = 0x0155d510;
BV300U = 0x01387f10;
BV300J = 0x01103710;

BASE = (IS_V300E ? BV300E : (
        IS_V300U ? BV300U : (
        IS_V300J ? BV300J : 0
        )));

SECTIONS
{
    .text BASE : {
        *(.text.boot)
        *(.text*)
    }
    .rodata : { *(.rodata*) }
    .data : { *(.data*) }
    .bss : { *(.bss*) *(COMMON) }
}
