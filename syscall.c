#include "syscall.h"
#include "threads.h"
#include "serial.h"


static int write(const char *buf, size_t size)
{
    serial_write(buf, size);
    return 0;
}

syscall_t syscall_table[NSYSCALLS] = {
    (syscall_t)write,
//    (syscall_t)getpid,
//    (syscall_t)exit,
//    (syscall_t)wait,
    (syscall_t)fork,
//    (syscall_t)exec
};

