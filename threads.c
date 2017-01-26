#include "stdatomic.h"
#include "stdint.h"

#include "threads.h"
#include "string.h"
#include "alloc.h"
#include "time.h"
#include "debug.h"
#include "desc.h"


typedef struct threads_regs_t {
	uint64_t r15;
	uint64_t r14;
	uint64_t r13;
	uint64_t r12;
	uint64_t rbp;
	uint64_t rbx;
	uint64_t r11;
	uint64_t r10;
	uint64_t r9;
	uint64_t r8;
	uint64_t rax;
	uint64_t rcx;
	uint64_t rdx;
	uint64_t rsi;
	uint64_t rdi;
	uint64_t intno;
	uint64_t error;
	uint64_t rip;
	uint64_t cs;
	uint64_t rflags;
	uint64_t rsp;
	uint64_t ss;
} threads_regs_t;

struct thread_switch_frame {
	uint64_t rflags;
	uint64_t r15;
	uint64_t r14;
	uint64_t r13;
	uint64_t r12;
	uint64_t rbx;
	uint64_t rbp;
	uint64_t rip;
	threads_regs_t regs;
} __attribute__((packed));


#define IO_MAP_BITS  (1 << 16)
#define IO_MAP_WORDS (IO_MAP_BITS / sizeof(unsigned long))

typedef struct tss_t {
	uint32_t rsrv0;
	uint64_t rsp[3];
	uint64_t rsrv1;
	uint64_t ist[7];
	uint64_t rsrv2;
	uint16_t rsrv3;
	uint16_t iomap_base;
	unsigned long iomap[IO_MAP_WORDS + 1]; // offset = 104
} __attribute__((packed)) tss_t;


static const size_t STACK_ORDER = 1;
static const unsigned long long SLICE = 10;

static struct mem_cache thread_cache;
static struct thread *current;
static struct thread *idle_thread;
static struct thread *kthread;
static struct list_head runqueue;
static struct spinlock runqueue_lock;
static atomic_int preempt_cnt;
static struct tss_t tss;

static pid_t init_thread_pid()
{
	static pid_t current_pid = 0;
	return current_pid++;
}

static size_t __thread_stack_size()
{
	return (size_t)1 << (PAGE_SHIFT + STACK_ORDER);
}

void *thread_stack_begin(struct thread *thread)
{
	extern char bootstrap_stack_bottom[];
	if (thread == kthread)
	{
 		return bootstrap_stack_bottom;
	}
	return (void *)thread->stack_addr;
}

void *thread_stack_end(struct thread *thread)
{
	extern char bootstrap_stack_top[];
	if (thread == kthread)
	{
 		return bootstrap_stack_top;
	}
	return (void *)((uintptr_t)thread_stack_begin(thread) + __thread_stack_size());
}

static threads_regs_t *__current_thread_regs()
{
	struct thread *thread = thread_current();
	return (threads_regs_t *)((uint64_t)thread_stack_end(thread) - sizeof(threads_regs_t));
}

static struct thread *__thread_alloc()
{
	return mem_cache_alloc(&thread_cache);
}

static struct thread *thread_alloc(void)
{
	struct page *stack = __page_alloc(STACK_ORDER);

	if (!stack)
	{
 		BUG("stack allocation failed");
 		return 0;
	}

	struct thread *thread = __thread_alloc();

	BUG_ON(thread == 0);

	thread->stack = stack;
	thread->stack_order = STACK_ORDER;
	thread->stack_addr = (uintptr_t)va(page_addr(stack));
	thread->stack_ptr = thread->stack_addr + __thread_stack_size()
 	 	  - sizeof(struct thread_switch_frame);

	memset((void *)thread->stack_addr, 0, __thread_stack_size());
	thread->pid = init_thread_pid();
	return thread;
}

static void thread_free(struct thread *thread)
{
	mem_cache_free(&thread_cache, thread);
}

void disable_preempt(void)
{
	atomic_fetch_add_explicit(&preempt_cnt, 1, memory_order_relaxed);
}

void enable_preempt(void)
{
	atomic_fetch_sub_explicit(&preempt_cnt, 1, memory_order_relaxed);
}

struct tss_desc {
	uint64_t low;
	uint64_t high;
} __attribute((packed)) tss_desc;

static void setup_tss_desc(struct tss_desc *desc, struct tss_t *tss)
{
	const uint64_t limit = sizeof(*tss) - 1;
	const uint64_t base = (uint64_t)tss;

	desc->low = (limit & 0xFFFFul) | ((base & 0xFFFFFFul) << 16)
 	 	  | 0xE90000000000 |
 	 	  // 0b11101001 << 40 |
 	 	  // | 0x890000000000 |
 	 	  ((limit & (0xF0000ul << 16)) << 32) // 89 = (0x9ul | (1 << 7)) << 40), (0xFul << 16)
 	 	  | ((base & (0xFFul << 24)) << 32);
	desc->high = (base >> 32) &  0xFFFFFFFFul;
}


static void tss_setup(void)
{
	extern char bootstrap_stack_top[];
	const size_t tss_entry = TSS >> 3;
	uint64_t *gdt = gdtr_ptr();

	tss.iomap_base = 104;
	tss.rsp[0] = (uint64_t)bootstrap_stack_top;

	memset(tss.iomap, 0xff, sizeof(tss.iomap));
	setup_tss_desc((struct tss_desc *)(gdt + tss_entry), &tss);

	const uint16_t rpl3_flag = 3;
	const uint16_t tss_selector = TSS | rpl3_flag;
	__asm__ volatile ("ltr %0" : : "a"(tss_selector));
}

void threads_setup(void)
{
	const size_t size = sizeof(struct thread);
	const size_t align = 64;

	mem_cache_setup(&thread_cache, size, align);
	spin_setup(&runqueue_lock);
	list_init(&runqueue);

	tss_setup();

	current = __thread_alloc();
	kthread = current;

	BUG_ON(!current);
	memset(current, 0, sizeof(current));
	current->state = THREAD_ACTIVE;
}

static void place_thread(struct thread *next)
{
	struct thread *prev = thread_current();

	if (prev->state == THREAD_FINISHING)
		prev->state = THREAD_FINISHED;

	tss.rsp[0] = (uint64_t)thread_stack_end(prev);

	spin_lock(&runqueue_lock);
	if (prev->state == THREAD_ACTIVE)
		list_add_tail(&prev->ll, &runqueue);
	spin_unlock(&runqueue_lock);

	current = next;
	next->time = current_time();
}

void thread_entry(struct thread *thread, void (*fptr)(void *), void *arg)
{
	place_thread(thread);
	enable_ints();

	fptr(arg);
	thread_exit();
}


struct thread *thread_create(void (*fptr)(void *), void *arg)
{
 	extern void __thread_entry(void);

	struct thread *thread = thread_alloc();
	thread->state = THREAD_BLOCKED;

	struct thread_switch_frame *frame =
 	 	  (struct thread_switch_frame *)thread->stack_ptr;
	frame->r12 = (uintptr_t)thread;
	frame->r13 = (uintptr_t)fptr;
	frame->r14 = (uintptr_t)arg;
	frame->rbp = 0;
	frame->rflags = (1ul << 2);
	frame->rip = (uintptr_t)__thread_entry;

	frame->regs.ss = (uint64_t)KERNEL_DS;
	frame->regs.cs = (uint64_t)KERNEL_CS;
	frame->regs.rsp = (uint64_t)(thread_stack_end(thread));
	frame->regs.rip = (uint64_t)&thread_exit;

	return thread;
}

static struct thread *__thread_fork()
{
	extern void __thread_entry(void);

	struct thread *thread = thread_alloc();

	thread->state = THREAD_BLOCKED;

	struct thread_switch_frame *frame =
 		(struct thread_switch_frame *)(thread->stack_ptr);
	frame->r12 = (uintptr_t)thread;
	frame->r13 = 0;
	frame->r14 = 0;
	frame->rbp = 0;
//    frame->rflags = (1ul << 2);
	frame->rip = (uintptr_t)__thread_entry;

	memcpy(&frame->regs, __current_thread_regs(), sizeof(threads_regs_t));
	frame->regs.rip = (uintptr_t)__thread_entry;
	frame->regs.rax = 0; // return code.

	return thread;
}

pid_t fork()
{
	struct thread *thread = __thread_fork();
	BUG_ON(!thread);
	thread_activate(thread);
	printf("Hello #%d\n", getpid());
	return thread->pid;
}

void thread_exec(void (*fptr)(void *), void *arg)
{
	struct thread *thread = thread_create(fptr, arg);
	thread_activate(thread);
	thread_join(thread);
	thread_destroy(thread);
}

void thread_destroy(struct thread *thread)
{
	__page_free(thread->stack, thread->stack_order);
	thread_free(thread);
}

struct thread *thread_current(void)
{ return current; }

void thread_activate(struct thread *thread)
{
	const int enable = spin_lock_irqsave(&runqueue_lock);
	if (thread->state == THREAD_BLOCKED) {
		thread->state = THREAD_ACTIVE;
		list_add_tail(&thread->ll, &runqueue);
	}
	spin_unlock_irqrestore(&runqueue_lock, enable);
}

void thread_exit(void)
{
	struct thread *self = thread_current();

	self->state = THREAD_FINISHING;
	while (1)
		force_schedule();
}

void thread_join(struct thread *thread)
{
	while (thread->state != THREAD_FINISHED)
		force_schedule();
}

static void thread_switch_to(struct thread *next)
{
	void __thread_switch(uintptr_t *prev_state, uintptr_t next_state);

	struct thread *self = thread_current();

	printf("switch: %d -> %d\n", self->pid, next->pid);
	__thread_switch(&self->stack_ptr, next->stack_ptr);
	place_thread(self);
}

static void __schedule(int force)
{
	const unsigned long long time = current_time();
	struct thread *prev = thread_current();
	struct thread *next = idle_thread;

	if (!force && prev->state == THREAD_ACTIVE && time - prev->time < SLICE)
		return;

	spin_lock(&runqueue_lock);
	if (!list_empty(&runqueue)) {
		next = (struct thread *)list_first(&runqueue);
		list_del(&next->ll);
	}
	spin_unlock(&runqueue_lock);

	if (prev == next)
		return;

	if (next && (next != idle_thread || prev->state != THREAD_ACTIVE)) {
		BUG_ON(!next);
		thread_switch_to(next);
	}
}

void schedule(void)
{
	if (atomic_load_explicit(&preempt_cnt, memory_order_relaxed))
		return;

	const int enable = ints_enabled();

	disable_ints();
	__schedule(0);

	if (enable) enable_ints();
}

void force_schedule(void)
{
	BUG_ON(atomic_load_explicit(&preempt_cnt, memory_order_relaxed));

	const int enable = ints_enabled();

	disable_ints();
	__schedule(1);

	if (enable) enable_ints();
}

void idle(void)
{
	struct thread *self = thread_current();

	BUG_ON(idle_thread);
	idle_thread = self;
	self->state = THREAD_IDLE;
	while (1)
		force_schedule();
}
