#include "serial.h"
#include "memory.h"
#include "balloc.h"
#include "paging.h"
#include "debug.h"
#include "alloc.h"
#include "print.h"
#include "ints.h"
#include "time.h"
#include "threads.h"
#include "mutex.h"
#include "condition.h"

#include "initramfs.h"
#include "ramfs.h"
#include "defines.h"
#include "string.h"

#include "syscall.h"

static void qemu_gdb_hang(void)
{
#ifdef DEBUG
    static volatile int wait = 1;

    while (wait);
#endif
}

void ramfs_tests()
{}


void syscall_tests()
{
    printf("Starting system call tests!\n");
    static const char *str = "Hello, syscall!\n";
    const size_t n = strlen(str);
    extern void write_usermode(const char *str, const size_t n);
    write_usermode(str, n);
}

void main(struct multiboot_info *mboot_info)
{
    qemu_gdb_hang();

    serial_setup();
    ints_setup();
    time_setup();
    find_initrd(mboot_info);
    balloc_setup(mboot_info);
    paging_setup();
    page_alloc_setup();
    mem_alloc_setup();
    kmap_setup();
    threads_setup();
    enable_ints();

    syscall_tests();

    printf("===The end.===\n");
    idle();
}
