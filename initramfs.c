#include "defines.h"
#include "initramfs.h"
#include "alloc.h"
#include "string.h"
#include "serial.h"
#include "ramfs.h"

uintptr_t initrd_begin = 0;
uintptr_t initrd_end = 0;

uint32_t pad4(uint32_t size)
{
    while (size % 4)
    {
        size++;
    }
    return size;
}

uint32_t cpiotoi(const char *buf, const size_t size)
{
    uint32_t res = 0, mul = 1;
    for (size_t i = 0; i < size; ++i)
    {
        res += mul * char_to_digit(buf[size-i-1]);
        mul *= 16;
    }
    return res;
}


void write_cpio_entities()
{
    const char *ptr = (const char *)initrd_begin;
    for (;;)
    {
        const cpio_header *cpio_ptr = (cpio_header *)ptr;
        const char *filename = ptr + sizeof(cpio_header);
        const uint32_t namesize = cpiotoi(cpio_ptr->namesize, 8);
        serial_write(filename, namesize);
        const uint32_t filesize = cpiotoi(cpio_ptr->filesize, 8);

        serial_write_terminated("\nFile begins\n");
        serial_write_terminated("CPIO's header: ");
        serial_write((const char *)cpio_ptr, sizeof(cpio_header));
        serial_write_terminated("\nFile ends\n");

        if (str_compare(filename, END_OF_ARCHIVE, 10) == 0)
        {
            serial_write_terminated("It's over");
            break;
        }
        ptr += pad4(sizeof(cpio_header) + namesize) + pad4(filesize);
    }
}


void find_initrd(struct multiboot_info *mboot_info)
{
    if (!(mboot_info->flags & (1 << 3)))
    {
        serial_write_terminated("multiboot header's flag[3] not set");
    }

    size_t mods_count = mboot_info->mods_count;
    multiboot_module_t *modules = (multiboot_module_t *)((uintptr_t)mboot_info->mods_addr);
    for (size_t i = 0; i < mods_count; ++i)
    {
        if (str_compare((char *)((uintptr_t)modules[i].mod_start), "070701", 6) == 0)
        {
            serial_write_terminated("LOG: initramfs module found!\n");
            initrd_begin = (uintptr_t)modules[i].mod_start;
            initrd_end = (uintptr_t)(initrd_begin + modules[i].mod_end);
        }
    }
}



void initramfs()
{
    const char *ptr = (const char *)initrd_begin;
    for (;;)
    {
        const cpio_header *cpio_ptr = (cpio_header *)ptr;
        const char *filename = ptr + sizeof(cpio_header);
        if (str_compare(filename, END_OF_ARCHIVE, strlen(END_OF_ARCHIVE)) == 0)
        {
            serial_write_terminated("Initramfs is loaded\n");
            serial_write_terminated("--------------------------------------------------------------");
            break;
        }
#ifdef __DEBUG
        serial_write_terminated(filename);
        serial_write_terminated("\n");
#endif
        const size_t filename_size = cpiotoi(cpio_ptr->namesize, 8);

        const uint32_t mode = cpiotoi(cpio_ptr->mode, 8);
        const bool is_dir = S_ISDIR(mode);
        const char *contents = ptr + pad4(sizeof(cpio_header) + filename_size);
        fentry *f = ramfs_create_file(filename, is_dir);

        const uint32_t filesize = cpiotoi(cpio_ptr->filesize, 8);
        if (filesize > 0 && is_dir)
        {
            serial_write_terminated("ERROR: directory \"");
            serial_write_terminated(filename);
            serial_write_terminated("\" is not empty");
            return;
        }
        if (!is_dir)
        {
            fentry *ff = ramfs_open(filename);
            if (ff != f)
            {
                serial_write_terminated("Internal error while loading file: ");
                serial_write_terminated(filename);
                return;
            }
            ramfs_write(ff, 0, contents, filesize);
            ramfs_close(ff);
        }

        ptr += pad4(sizeof(cpio_header) + filename_size) + pad4(filesize);
    }
}


// TODO: Move to main.

