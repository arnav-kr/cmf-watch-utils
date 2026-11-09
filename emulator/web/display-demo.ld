SECTIONS
{
    . = 0x10000200;
    .vectors : { KEEP(*(.vectors)) }
    .text : { *(.text*) *(.rodata*) }
    /DISCARD/ : { *(.comment) *(.ARM.attributes) }
}
