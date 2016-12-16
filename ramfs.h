#ifndef __RAMFS_H__
#define __RAMFS_H__

#include <stdbool.h>
#include <stddef.h>
#include "mutex.h"


typedef struct fentry
{
    char *filename;
    struct fentry *parent, *next;

    struct fentry_list *children; // directory only (is_dir = true)
    char *contents; size_t size;
    bool is_dir;

    mutex *m;
} fentry;


typedef struct fentry_list
{
    fentry *begin, *end;
} fentry_list;


fentry *ramfs_create_file(const char *filename, bool is_dir);


fentry *ramfs_find(const char *filename);


fentry *ramfs_mkdir(const char *filename);


void ramfs_readdir(const char *filename);


fentry *ramfs_open(const char *filename);


void ramfs_close(fentry *f);


void ramfs_read(fentry *f, size_t seek);


void ramfs_write(fentry *f, size_t seek, const char *contents, const size_t size);


void ramfs_tests();


#endif // __RAMFS_H__
