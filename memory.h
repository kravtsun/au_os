#ifndef __MEMORY_H__
#define __MEMORY_H__

#include "defines.h"

#ifndef __ASM_FILE__

#include "stdint.h"
#include "list.h"
#include "spinlock.h"


static inline void *va(uintptr_t phys)
{ return (void *)(phys + HIGHER_BASE); }

static inline uintptr_t pa(const void *virt)
{ return (uintptr_t)virt - HIGHER_BASE; }

struct mem_cache;

struct page {
	struct list_head ll;
	unsigned long flags;
	union {
		int order;
		struct mem_cache *cache;
	} u;
};

struct page_alloc_zone {
	struct spinlock lock;
	struct list_head ll;
	uintptr_t begin;
	uintptr_t end;
	struct list_head order[MAX_ORDER + 1];
	struct page pages[1];
};

uintptr_t page_addr(const struct page *page);
struct page *addr_page(uintptr_t addr);

void page_set_bit(struct page *page, int bit);
void page_clear_bit(struct page *page, int bit);
int page_test_bit(const struct page *page, int bit);

void page_alloc_setup(void);
struct page *__page_alloc(int order);
uintptr_t page_alloc(int order);
void __page_free(struct page *page, int order);
void page_free(uintptr_t addr, int order);

#endif /*__ASM_FILE__*/

#endif /*__MEMORY_H__*/
