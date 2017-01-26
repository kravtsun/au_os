// commo*n.h -- Defines typedefs and some global functions.

#ifndef COMMON_H
#define COMMON_H

typedef unsigned long long uint64_t;
typedef          long long int64_t;
typedef unsigned int       uint32_t;
typedef          int       int32_t;
typedef unsigned short     uint16_t;
typedef          short     int16_t;
typedef unsigned char      uint8_t;
typedef          char      int8_t;

// Write a byte out to the specified port.
void outb(uint16_t port, uint8_t value);

// Read a byte from the specified port.
uint8_t inb(uint16_t port);

// Read a word from the specified port.
uint16_t inw(uint16_t port);

// Write len copies of val into dest.
void memset(uint8_t *dest, uint8_t val, uint32_t len);

void memcpy(void *_dst, const void *_src, uint32_t size);

int memcmp(const void *_lhs, const void *_rhs, uint32_t size);


typedef struct RegistersContainer
{
    uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax; // pushal
    uint32_t int_no, err_code;
    uint32_t eip, cs, eflags, useresp, ss;
} RegistersContainer;


// состояние процесса
typedef struct state_t {
//    uint32_t state_new, state_old;
    RegistersContainer registers;
} state_t;



static inline int are_interrupts_enabled()
{
    unsigned long flags;
    asm volatile ( "pushf\n\t"
                   "pop %0"
                   : "=g"(flags) );
    return flags & (1 << 9);
}


static inline int bit(uint32_t num, int i) {
    return num & (1 << i);
}


#endif // COMMON_H
