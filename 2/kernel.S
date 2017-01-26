.code32
# Here is the executable code located
.text

.globl  _start
_start:
jmp     multiboot_entry

# Align 32 bits boundary.
.align  4
# maybe 3 for page alignment? TODO: should be 0x00000002
#define MULTIBOOT_HEADER_FLAGS 0x00000003

# Multiboot header.
multiboot_header:
    /* magic */
    .long   0x1BADB002
    /* flags */
    .long   MULTIBOOT_HEADER_FLAGS 
    /* checksum */
    .long   -(0x1BADB002 + MULTIBOOT_HEADER_FLAGS)

    # ### ELF ###
    # /* header_addr */
    # .long   multiboot_header
    # /* load_addr */
    # .long   _start
    # /* load_end_addr */
    # .long   _edata
    # /* bss_end_addr */
    # .long   _end
    # /* entry_addr */
    # .long   multiboot_entry
    # ### ELF ###

multiboot_entry:
    # Initialize the stack pointer.
    movl    $(stack + 0x20000), %esp

    # Reset EFLAGS.
    pushl   $0
    popf

    /* Push the pointer to the Multiboot information structure. */
    pushl   %ebx
    /* Push the magic value. */
    pushl   %eax

    # Now enter the C main function...
    call    main

    loop:   hlt
    jmp     loop

    .bss
    # Our stack area.
    .comm   stack, 0x10000

