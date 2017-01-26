
# interrupt.S -- Contains interrupt service routine wrappers.

# This macro creates a stub for an ISR which does NOT pass it's own
# error code (adds a dummy errcode byte).
.macro ISR_NOERRCODE num
   .globl isr\num
  isr\num:
    push $0                # Put dummy error code in case if ISR does not.
    push $\num             # Put interrupt number.
    jmp isr_common_stub    # Go to common handler's code.
.endm

.macro ISR_ERRCODE num
  .globl isr\num
  isr\num:
    push $\num             # Push the interrupt number
    jmp isr_common_stub
.endm

# This macro creates a stub for an IRQ - the first parameter is
# the IRQ number, the second is the ISR number it is remapped to.
.macro IRQ irq_from, isr_to
  .globl irq\irq_from
  irq\irq_from:
    push $0
    push $\isr_to
    jmp irq_common_stub
.endm


# Interruptions 8, 10-14 put error code into stack, others don't.
ISR_NOERRCODE 0
ISR_NOERRCODE 1
ISR_NOERRCODE 2
ISR_NOERRCODE 3
ISR_NOERRCODE 4
ISR_NOERRCODE 5
ISR_NOERRCODE 6
ISR_NOERRCODE 7
ISR_ERRCODE   8
ISR_NOERRCODE 9
ISR_ERRCODE   10
ISR_ERRCODE   11
ISR_ERRCODE   12
ISR_ERRCODE   13
ISR_ERRCODE   14
ISR_NOERRCODE 15
ISR_NOERRCODE 16
ISR_NOERRCODE 17
ISR_NOERRCODE 18
ISR_NOERRCODE 19
ISR_NOERRCODE 20
ISR_NOERRCODE 21
ISR_NOERRCODE 22
ISR_NOERRCODE 23
ISR_NOERRCODE 24
ISR_NOERRCODE 25
ISR_NOERRCODE 26
ISR_NOERRCODE 27
ISR_NOERRCODE 28
ISR_NOERRCODE 29
ISR_NOERRCODE 30
ISR_NOERRCODE 31
IRQ   0,    32
IRQ   1,    33
IRQ   2,    34
IRQ   3,    35
IRQ   4,    36
IRQ   5,    37
IRQ   6,    38
IRQ   7,    39
IRQ   8,    40
IRQ   9,    41
IRQ  10,    42
IRQ  11,    43
IRQ  12,    44
IRQ  13,    45
IRQ  14,    46
IRQ  15,    47

# In isr.c
.extern isr_handler

# This is our common ISR stub. It saves the processor state, sets
# up for kernel mode segments, calls the C-level fault handler,
# and finally restores the stack frame.
isr_common_stub:
    call isr_handler
    addl $8, %esp   # Cleans up the pushed error code and pushed ISR number
    iret            # pops 5 things at once: CS, EIP, EFLAGS, SS, and ESP


# In isr.c
.extern irq_handler

# This is our common IRQ stub. It saves the processor state, sets
# up for kernel mode segments, calls the C-level fault handler,
# and finally restores the stack frame.
irq_common_stub:
    call irq_handler

    addl $8, %esp   # Cleans up the pushed error code and pushed ISR number

    iret            # pops 5 things at once: CS, EIP, EFLAGS, SS, and ESP

