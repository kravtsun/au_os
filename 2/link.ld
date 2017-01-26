OUTPUT_FORMAT(elf32-i386)
OUTPUT_ARCH(i386)
ENTRY(_start)

SECTIONS
{
    . = 0x100000 + SIZEOF_HEADERS;
    .text : { *(.text) }
    .data : { *(.data) }
    .bss  : { *(.bss)  }
}
