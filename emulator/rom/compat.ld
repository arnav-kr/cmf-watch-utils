ENTRY(memcpy)
SECTIONS {
  /* Leopard ROM family signature consumed by the SDK audio library. */
  .rom_id_low 0x140 : { LONG(0x00005128) }
  .rom_id_high 0x148 : { LONG(0x69740000) }
  .brom_api 0x188 : { KEEP(*(.brom_api)) }
  .spinor_api 0x200 : { KEEP(*(.spinor_api)) }
  .libc_api 0x7030 : { KEEP(*(.libc_api)) }
  .misc_api 0x7090 : { KEEP(*(.misc_api)) }
  .gui_api 0x70a0 : { KEEP(*(.gui_api)) }
  .health_magic 0x7efc : { LONG(0x57503248) }
  .health_entry 0x7f00 : { KEEP(*(.health_entry)) }
  .text 0x8000 : { *(.text*) *(.rodata*) }
  .ARM.exidx : { *(.ARM.exidx*) }
  .data : { *(.data*) }
  .bss : { *(.bss*) *(COMMON) }
  ASSERT(. < 0x10000, "Compatibility ROM exceeds mapped ROM")
}
