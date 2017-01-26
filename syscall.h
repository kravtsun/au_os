#ifndef __SYSCALL_H__
#define __SYSCALL_H__

#define NSYSCALLS 2

#ifndef __ASM_FILE__

typedef int (*syscall_t )(); // system call function type.
extern syscall_t syscall_table[NSYSCALLS];

#endif //__ASM_FILE__



#endif //__SYSCALL_H__
