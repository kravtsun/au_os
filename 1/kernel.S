.code32
# Here is the executable code located
.text

.globl  _start
_start:
jmp     multiboot_entry

# Align 32 bits boundary.
.align  4

# Multiboot header.
multiboot_header:
    /* magic */
    .long   0x1BADB002
    /* flags */
    .long   0x00000000
    /* checksum */
    .long   -(0x1BADB002 + 0x00000000)
    /* header_addr */

multiboot_entry:
    # Initialize the stack pointer.
    movl    $(stack + 0x4000), %esp

    # Reset EFLAGS.
    pushl   $0
    popf

    # Now enter the C main function...
    call    main

    loop:   hlt
    jmp     loop

    .bss
    # Our stack area.
    .comm   stack, 0x4000

