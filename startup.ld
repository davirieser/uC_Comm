
MEMORY
{
  RAM    (xrw)    : ORIGIN = 0x20000000,   LENGTH = 4K
  FLASH   (rx)    : ORIGIN = 0x08000000,   LENGTH = 16K
}

_estack = ORIGIN(RAM) + LENGTH(RAM);	/* end of "RAM" Ram type memory */
_stack = _estack;

SECTIONS
{
    .text : { *(.text*) } > FLASH
    .rodata : { *(.rodata*) } > FLASH
    .data : { *(.data*) } > RAM
    .bss : { *(.bss*) *(COMMON)} > RAM
}
