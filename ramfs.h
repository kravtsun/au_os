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

    mutex *mtx;
} fentry;


typedef struct fentry_list
{
    fentry *begin;
} fentry_list;


fentry *ramfs_create_file(const char *filename, bool is_dir);


fentry *ramfs_find(const char *filename);


fentry *ramfs_mkdir(const char *filename);


void ramfs_readdir(const char *filename);


fentry *ramfs_open(const char *filename);


void ramfs_close(fentry *f);


size_t ramfs_read(const fentry *f, size_t seek, void *buf, size_t count);


size_t ramfs_write(fentry *f, size_t seek, const void *contents, const size_t count);


void ramfs_tests();


#endif // __RAMFS_H__
