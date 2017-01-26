#ifndef __THREADS_H__
#define __THREADS_H__

#include "stdint.h"

#include "list.h"

enum thread_state {
	THREAD_IDLE,
	THREAD_ACTIVE,
	THREAD_BLOCKED,
	THREAD_FINISHING,
	THREAD_FINISHED
};

typedef int32_t pid_t;
struct thread {
	struct list_head ll;
	enum thread_state state;
	unsigned long long time;
	struct page *stack;
	uintptr_t stack_addr;
	int stack_order;
	uintptr_t stack_ptr;
    pid_t pid;
};


struct thread *__thread_create(void (*fptr)(void *), void *arg);
struct thread *thread_create(void (*fptr)(void *), void *arg);
void thread_exec(void (*fptr)(void *), void *arg);
void thread_destroy(struct thread *thread);

struct thread *thread_current(void);
void thread_activate(struct thread *thread);
void thread_join(struct thread *thread);
void thread_exit(void);

void *thread_stack_begin(struct thread *thread);
void *thread_stack_end(struct thread *thread);

pid_t fork();

static inline pid_t getpid() { return thread_current()->pid; }

void disable_preempt(void);
void enable_preempt(void);

void schedule(void);
void force_schedule(void);
void idle(void);

void threads_setup(void);

#endif /*__THREADS_H__*/
