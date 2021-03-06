#ifndef __BOOTSTRAP_ALLOCATOR_H__
#define __BOOTSTRAP_ALLOCATOR_H__

#include "stdint.h"
#include "stddef.h"
#include "rbtree.h"
#include "list.h"
#include "defines.h"

struct memory_node {
	union {
		struct rb_node rb;
		struct list_head ll;
	} link;
	unsigned long long begin;
	unsigned long long end;
};

extern struct rb_tree memory_map;
extern struct rb_tree free_ranges;

void balloc_setup(const multiboot_info *info);
uintptr_t __balloc_alloc(size_t size, uintptr_t align,
			uintptr_t from, uintptr_t to);
uintptr_t balloc_alloc(size_t size, uintptr_t from, uintptr_t to);
void balloc_free(uintptr_t begin, uintptr_t end);

uintptr_t balloc_memory(void);

static inline struct memory_node *RB2MEMORY_NODE(const struct rb_node *node)
{
	return CONTAINER_OF(node, struct memory_node, link);
}

static inline struct memory_node *LL2MEMORY_NODE(const struct list_head *node)
{
	return CONTAINER_OF(node, struct memory_node, link);
}

#endif /*__BOOTSTRAP_ALLOCATOR_H__*/
