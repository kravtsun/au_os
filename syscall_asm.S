#define ASM_FILE
#include "defines.h"
#undef __ASM_FILE__
	.global write_usermode
	.extern kuku
write_usermode:
    mov $(USER_DS | 0x03), %rax
	mov %ax, %ds
	mov %ax, %es
	mov %ax, %fs
	mov %ax, %gs

    movq %rsp, %rax

    push $(USER_DS | 0x03) //SS
    push %rax  //SP

    pushfq     //EFLAGS
    push $(USER_CS | 0x03) //CS

    leaq write_call, %rax
    pushq %rax //RIP

    iretq

    .global write_call
write_call:
    xorq %rax, %rax // choose write buffer from %rdi, length %rsi
    int $0x80
    1: jmp 1b
