#include "ramfs.h"
#include "defines.h"

#include "alloc.h"
#include "string.h"
#include "serial.h"


#define RAMFS_MAX_FILENAME_SIZE 256
//static char ramfs_filename_buf[RAMFS_MAX_FILENAME_SIZE];

char *trim_filename(const char *filename)
{
    size_t size = strlen(filename);
    if (filename[size-1] == '/') size--;
    char *fn = mem_alloc(size+1);
    fn[size] = '\0';
    memcpy(fn, filename, size);
    return fn;
}

static fentry_list *fentry_list_create();
static void fentry_list_release(fentry_list *fl);
static fentry *fentry_list_add(fentry_list *fl, fentry *f);
static bool fentry_equals_filename(fentry *f, const char *filename);

fentry *__root = NULL;

// TODO write a decent function to signalize error (not through copy-paste serial_write_terminated).

fentry *fentry_create(const char *filename, fentry *parent, bool is_dir)
{
    fentry *res = (fentry *)mem_alloc(sizeof(fentry));
    res->filename = trim_filename(filename);
    res->parent = parent;
    if (parent == NULL && __root == NULL)
    {
        __root = res;
    }
    else
    {
        if (parent == NULL)
        {
            serial_error_message("Internal error when creating an entry.\n","", "");
        }
        mutex_lock(parent->mtx);
        res = fentry_list_add(parent->children, res);
        mutex_unlock(parent->mtx);
        if (res == NULL)
        {
            mem_free(res);
            return NULL;
        }
    }
    res->next = NULL;

    res->children = fentry_list_create();
    res->contents = NULL;
    res->size = 0;
    res->is_dir = is_dir;

    res->mtx = mem_alloc(sizeof(mutex));
    mutex_setup(res->mtx);
    return res;
}

void fentry_release(fentry *f)
{
    if (f == NULL) return;
    if (f->mtx->locked)
    {
        serial_error_message("ERROR: file: ",
                             f->filename, " is still busy. \n");
        return;
    }
    mem_free(f->mtx);
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
        serial_error_message("ERROR: trying to set directory: ",
                             f->filename, " with file contents\n");
        return;
    }
    mem_free(f->contents);
    f->contents = mem_alloc(size);
    f->size = size;

    memcpy(f->contents, contents, size);
}


static void fentry_list_print(const fentry_list *fl);

static void fentry_print(const fentry *f)
{
    serial_write_terminated(f->filename);
    serial_write_terminated("\n");
    fentry_list_print(f->children);
}

fentry_list *fentry_list_create()
{
    fentry_list *fl = mem_alloc(sizeof(fentry_list));
    fl->begin = NULL;
    return fl;
}

fentry *fentry_list_add(fentry_list *fl, fentry *f)
{
    fentry *it = fl->begin;
    f->next = NULL;
    if (it == NULL)
    {
        fl->begin = f;
    }
    else
    {
        fentry *end = NULL;
        while (it)
        {
            end = it;
            if (fentry_equals_filename(it, f->filename))
            {
                serial_error_message("File already exist: ", f->filename, "\n");
                return NULL;
            }
            it = it->next;
        }
        end->next = f;
    }
    return f;
}

//static void fentry_list_remove(fentry_list *fl, fentry *f)
//{
//    fentry *it = fl->begin, *prev = NULL;
//    while (it)
//    {
//        if (it == f)
//        {
//            if (prev)
//            {
//                prev->next = it->next;
//                if (it->next == NULL)
//                {
//                    fl->end = prev;
//                }
//            }
//            else // it == fl->begin
//            {
//                fl->begin = it->next;
//                if (it->next == NULL)
//                {
//                    fl->end = NULL;
//                }
//            }
//            fentry_release(it);
//            break;
//        }
//        prev = it;
//        it = it->next;
//    }
//}

static void fentry_list_release(fentry_list *fl)
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

static void fentry_list_print(const fentry_list *fl)
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

static fentry *fentry_list_find_with_name(fentry_list *fl, const char *filename)
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

static char *get_directory_name(const char *filename)
{
    char *fn = trim_filename(filename);
    const size_t size = strlen(fn);
    size_t index = RAMFS_MAX_FILENAME_SIZE + 1;
    for (size_t i = 0; i < size; ++i)
    {
        if (fn[i] == '/')
        {
            index = i;
        }
    }
    if (index > RAMFS_MAX_FILENAME_SIZE)
    {
        if (__root != NULL)
        {
            serial_error_message("ERROR: fn: \"",
                                 fn, "\"does not contain '/' symbol.\n");
            return NULL;
        }
        index = strlen(fn);
    }

    char *res = mem_alloc(index+1);
    res[index] = '\0';
    memcpy(res, fn, index);
    mem_free(fn);
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
        serial_error_message("ERROR: file: \"",
                             filename, "\"already exists.\n");
        return f;
    }

    // TODO let get_directory_name work through some static buffer.
    char *directory_name = get_directory_name(filename);
    fentry *parent = ramfs_find(directory_name);

    if (__root != NULL && parent == NULL)
    {
        serial_error_message("ERROR: directory: \"",
                             directory_name, "\"does not exist.\n");
        return NULL;
    }

    mem_free(directory_name);

    return fentry_create(filename, parent, is_dir);
}

static fentry *ramfs_find_recursive(fentry *f, const char *filename)
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
    char *fn = trim_filename(filename);
    fentry *res = NULL;
    if (fentry_equals_filename(__root, fn))
    {
        res = __root;
    }
    else
    {
        res = ramfs_find_recursive(__root, fn);
    }

    mem_free(fn);
    return res;
}


fentry *ramfs_mkdir(const char *filename)
{
    fentry *f = ramfs_find(filename);
    if (f)
    {
        serial_error_message("ERROR: directory: \"",
                             filename, "\"already exists.\n");
        return f;
    }

    f = ramfs_create_file(filename, false);
    if (f == NULL)
    {
        serial_error_message("ERROR: Failed to make directory:", f->filename, "\n");
    }
    return f;
}


void ramfs_readdir(const char *filename)
{
    fentry *f = ramfs_find(filename);
    if (f == NULL)
    {
        serial_error_message("ERROR: directory: \"",
                             filename, "\" does not exist.\n");
        return;
    }
    if (!f->is_dir)
    {
        serial_error_message("ERROR: file: \"",
                             filename, "\" is not a directory.\n");
        return;
    }
    mutex_lock(f->mtx);
    fentry_list_print(f->children);
    mutex_unlock(f->mtx);
}


fentry *ramfs_open(const char *filename)
{
    fentry *f = ramfs_find(filename);
    if (f == NULL)
    {
        f = ramfs_create_file(filename, false);
        if (f == NULL)
        {
            serial_error_message("ERROR: create: ", filename, " failed. \n");
            return NULL;
        }
    }

    mutex_lock(f->mtx);
    return f;
}

void ramfs_close(fentry *f)
{
    mutex_unlock(f->mtx);
}


void ramfs_read(fentry *f, size_t seek)
{
    if (!f->mtx->locked)
    {
        serial_error_message("ERROR: One has to open file: ",
                             f->filename,"before reading it.\n");
        return;
    }

    if (f == NULL)
    {
        serial_error_message("ERROR: cannot write to NULL file: ", "", "\n");
        return;
    }
    if (f->size <= seek)
    {
        serial_error_message("Size of file: \"", f->filename,
        "\" is too small to set seek at: ");
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
        serial_error_message("ERROR: Cannot write into directory: ", f->filename, "\n");
        return;
    }

    if (!f->mtx->locked)
    {
        serial_error_message("ERROR: One has to open file: ",
                             f->filename, "before writing into it.\n");
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
