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

#define __ROOTDIR__ "/home/kravtsun/Dropbox/AU/CPP"

void ramfs_tests()
{
    ramfs_open(__ROOTDIR__"/Семинары/dummy_file.hs");
    ramfs_mkdir(__ROOTDIR__"/Лекции/DUMMY_DIR");

    ramfs_readdir(__ROOTDIR__"/Лекции");
    serial_write_terminated("-------------------------------------------------------------------------------------------\n");

    ramfs_readdir(__ROOTDIR__"/Семинары");
    serial_write_terminated("-------------------------------------------------------------------------------------------\n");
//    ramfs_readdir(__ROOTDIR__);
//    serial_write_terminated("-------------------------------------------------------------------------------------------\n");
    ramfs_readdir(__ROOTDIR__"/cpp_notes.txt");

    const char *fname = __ROOTDIR__"/cpp_notes.txt";
    fentry *f = ramfs_open(fname);
    ramfs_read(f, 0);
    ramfs_write(f, 10, "Hello", 5);
    ramfs_read(f, 0);
    serial_write_terminated("---------------------------Fine del test---------------------------\n");
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

    initramfs();

    ramfs_tests();

    idle();
}
