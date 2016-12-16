#include "ramfs.h"
#include "defines.h"

#include "alloc.h"
#include "string.h"
#include "serial.h"


#define RAMFS_MAX_FILENAME_SIZE 256
//static char ramfs_filename_buf[RAMFS_MAX_FILENAME_SIZE];

fentry_list *fentry_list_create();
void fentry_list_release(fentry_list *fl);
void fentry_list_add(fentry_list *fl, fentry *f);

fentry *__root = NULL;

// TODO write a decent function to signalize error (not through copy-paste serial_write_terminated).

fentry *fentry_create(const char *filename, fentry *parent, bool is_dir)
{
    fentry *res = (fentry *)mem_alloc(sizeof(fentry));
    size_t filename_size = strlen(filename);
    if (filename[filename_size-1] == '/') filename_size--;
    res->filename = mem_alloc(filename_size+1);
    memcpy(res->filename, filename, filename_size);
    res->filename[filename_size] = '\0';
    res->parent = parent;
    if (parent == NULL && __root == NULL)
    {
        __root = res;
    }
    else
    {
        fentry_list_add(parent->children, res);
    }
    res->next = NULL;

    res->children = fentry_list_create();
    res->contents = NULL;
    res->size = 0;
    res->is_dir = is_dir;

    res->m = mem_alloc(sizeof(mutex));
    mutex_setup(res->m);
    return res;
}

void fentry_release(fentry *f)
{
    if (f == NULL) return;
    if (f->m->locked)
    {
        serial_write_terminated("ERROR: file: ");
        serial_write_terminated(f->filename);
        serial_write_terminated(" is still opened. \n");
        return;
    }
    mem_free(f->m);

    mem_free(f->filename);
    fentry_list_release(f->children);
    mem_free(f->children);
    mem_free(f->contents);
    f->size = 0;
    mem_free(f);
}


static void fentry_file_set_contents(fentry *f, const char *contents, const size_t size)
{
    if (f->is_dir && size > 0)
    {
        serial_write_terminated("ERROR: trying to set directory: ");
        serial_write_terminated(f->filename);
        serial_write_terminated(" with file contents\n");
        return;
    }
    mem_free(f->contents);
    f->contents = mem_alloc(size);
    f->size = size;

    memcpy(f->contents, contents, size);
}


void fentry_list_print(const fentry_list *fl);

void fentry_print(const fentry *f)
{
    serial_write_terminated(f->filename);
    serial_write_terminated("\n");
    fentry_list_print(f->children);
}

fentry_list *fentry_list_create()
{
    fentry_list *fl = mem_alloc(sizeof(fentry_list));
    fl->begin = NULL;
    fl->end = NULL;
    return fl;
}

void fentry_list_add(fentry_list *fl, fentry *f)
{
    fentry *it = fl->end;
    f->next = NULL;
    if (it == NULL)
    {
        fl->begin = fl->end = f;
    }
    else
    {
        while (it->next)
        {
            it = it->next;
        }
        it->next = f;
        fl->end = f;
    }
}

void fentry_list_remove(fentry_list *fl, fentry *f)
{
    fentry *it = fl->begin, *prev = NULL;
    while (it)
    {
        if (it == f)
        {
            if (prev)
            {
                prev->next = it->next;
                if (it->next == NULL)
                {
                    fl->end = prev;
                }
            }
            else // it == fl->begin
            {
                fl->begin = it->next;
                if (it->next == NULL)
                {
                    fl->end = NULL;
                }
            }
            fentry_release(it);
            break;
        }
        prev = it;
        it = it->next;
    }
}

void fentry_list_release(fentry_list *fl)
{
    fentry *it = fl->begin;
    while (it)
    {
        fentry *it_next = it->next;
        fentry_release(it);
        it = it_next;
    }
    mem_free(fl);
}

void fentry_list_print(const fentry_list *fl)
{
    const fentry *it = fl->begin;
    while (it)
    {
        fentry_print(it);
        it = it->next;
    }
}

bool fentry_equals_filename(fentry *f, const char *filename)
{
    if (strlen(f->filename) != strlen(filename)) return false;
    return str_compare(f->filename, filename, strlen(filename)) == 0;
}

fentry *fentry_list_find_with_name(fentry_list *fl, const char *filename)
{
    fentry *it = fl->begin;
    while(it)
    {
        if (fentry_equals_filename(it, filename))
        {
            break;
        }
        it = it->next;
    }
    return it;
}

char *get_directory_name(const char *filename)
{
    const size_t size = strlen(filename);
    size_t index = RAMFS_MAX_FILENAME_SIZE + 1;
    for (size_t i = 0; i < size; ++i)
    {
        if (filename[i] == '/')
        {
            index = i;
        }
    }
    if (index > RAMFS_MAX_FILENAME_SIZE)
    {
        if (__root != NULL)
        {
            serial_write_terminated("ERROR: filename: \"");
            serial_write_terminated(filename);
            serial_write_terminated("\"does not contain '/' symbol");
            return NULL;
        }
        index = strlen(filename);
    }

    char *res = mem_alloc(index+1);
    res[index] = '\0';
    memcpy(res, filename, index);
    return res;
}

fentry *ramfs_create_file(const char *filename, bool is_dir)
{
    if (strlen(filename) > RAMFS_MAX_FILENAME_SIZE)
    {
        serial_write_terminated("ERROR: too long file's absolute path!!");
        return NULL;
    }

    fentry *f = ramfs_find(filename);
    if (f)
    {
        serial_write_terminated("ERROR: file: \"");
        serial_write_terminated(filename);
        serial_write_terminated("\"already exists.");
        return f;
    }

    // TODO let get_directory_name work through some static buffer.
    char *directory_name = get_directory_name(filename);
    fentry *parent = ramfs_find(directory_name);

    if (__root != NULL && parent == NULL)
    {
        serial_write_terminated("ERROR: directory: \"");
        serial_write_terminated(directory_name);
        serial_write_terminated("\"does not exist.\n");
        return NULL;
    }

    mem_free(directory_name);

    return fentry_create(filename, parent, is_dir);
}

fentry *ramfs_find_recursive(fentry *f, const char *filename)
{
    if (fentry_equals_filename(f, filename)) return f;

    fentry *p = fentry_list_find_with_name(f->children, filename);
    if (p) return p;
    fentry *it = f->children->begin;
    while (it)
    {
        fentry *pp = ramfs_find_recursive(it, filename);
        if (pp) return pp;
        it = it->next;
    }
    return NULL;
}

fentry *ramfs_find(const char *filename)
{
    if (__root == NULL) // must be uninitialized root.
        return NULL;
    if (fentry_equals_filename(__root, filename)) return __root;

    return ramfs_find_recursive(__root, filename);
}


fentry *ramfs_mkdir(const char *filename)
{
    fentry *f = ramfs_find(filename);
    if (f)
    {
        serial_write_terminated("ERROR: directory: \"");
        serial_write_terminated(filename);
        serial_write_terminated("\"already exists.\n");
        return f;
    }
    return ramfs_create_file(filename, false);
}


void ramfs_readdir(const char *filename)
{
    fentry *f = ramfs_find(filename);
    if (f == NULL)
    {
        serial_write_terminated("ERROR: directory: \"");
        serial_write_terminated(filename);
        serial_write_terminated("\" does not exist.\n");
        return;
    }
    if (!f->is_dir)
    {
        serial_write_terminated("ERROR: file: \"");
        serial_write_terminated(filename);
        serial_write_terminated("\" is not a directory.\n");
        return;
    }
    fentry_list_print(f->children);
}


fentry *ramfs_open(const char *filename)
{
    fentry *f = ramfs_find(filename);
    if (f == NULL)
    {
        f = ramfs_create_file(filename, false);
    }
    mutex_lock(f->m);
    return f;
}

void ramfs_close(fentry *f)
{
    mutex_unlock(f->m);
}


void ramfs_read(fentry *f, size_t seek)
{
    if (!f->m->locked)
    {
        serial_write_terminated("ERROR: One has to open file: ");
        serial_write_terminated(f->filename);
        serial_write_terminated("before reading it.");
        return;
    }

    if (f == NULL)
    {
        serial_write_terminated("ERROR: cannot write to file: ");
        serial_write_terminated(f->filename);
        return;
    }
    if (f->size <= seek)
    {
        serial_write_terminated("Size of file: \"");
        serial_write_terminated(f->filename);
        serial_write_terminated("\" is too small to set seek at: ");
        serial_write_hex(seek);
        return;
    }
    if (f)
    {
        serial_write(f->contents + seek, f->size - seek);
    }
}


void ramfs_write(fentry *f, size_t seek, const char *contents, const size_t size)
{
    if (f->is_dir)
    {
        serial_write_terminated("ERROR: Cannot write into directory: ");
        serial_write_terminated(f->filename);
        return;
    }

    if (!f->m->locked)
    {
        serial_write_terminated("ERROR: One has to open file: ");
        serial_write_terminated(f->filename);
        serial_write_terminated("before writing into it.");
        return;
    }

    if (seek + size <= f->size)
    {
        memcpy(f->contents + seek, contents, size);
    }
    else
    {
        char *tmp_buffer = mem_alloc(seek+size);
        memcpy(tmp_buffer, f->contents, seek);
        memcpy(tmp_buffer + seek, contents, size);
        fentry_file_set_contents(f, tmp_buffer, seek + size);
        mem_free(tmp_buffer);
    }
}
