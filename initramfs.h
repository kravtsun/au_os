#ifndef __INITRAMFS_H__
#define __INITRAMFS_H__

#define S_IFMT	0xF000
#define S_IFDIR	0x4000
#define S_IFREG	0x8000

#define S_ISDIR(mode)	(((mode) & S_IFMT) == S_IFDIR)
#define S_ISREG(mode)	(((mode) & S_IFMT) == S_IFREG)

#define END_OF_ARCHIVE	"TRAILER!!!"

typedef struct cpio_header
{
    char magic[6];
    char inode[8];
    char mode[8];
    char uid[8];
    char gid[8];
    char nlink[8];
    char mtime[8];
    char filesize[8];
    char major[8];
    char minor[8];
    char rmajor[8];
    char rminor[8];
    char namesize[8];
    char check[8];
} __attribute__((packed)) cpio_header;


void find_initrd(struct multiboot_info *mboot_info);

void write_cpio_entities();

void initramfs();

#endif /*__INITRAMFS_H__*/
