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

static void qemu_gdb_hang(void)
{
#ifdef DEBUG
    static volatile int wait = 1;

    while (wait);
#endif
}

void ramfs_tests()
{}

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

    initramfs();

    ramfs_tests();

    idle();
}
