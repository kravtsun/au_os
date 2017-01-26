// common.c -- Некоторые глобальные функции.

#include "common.h"


void outb(uint16_t port, uint8_t value) {
    asm volatile ("outb %1, %0" : : "dN" (port), "a" (value));
}


uint8_t inb(uint16_t port) {
    uint8_t ret;
    asm volatile("inb %1, %0" : "=a" (ret) : "dN" (port));
    return ret;
}


uint16_t inw(uint16_t port) {
    uint16_t ret;
    asm volatile ("inw %1, %0" : "=a" (ret) : "dN" (port));
    return ret;
}


void memset(uint8_t *dest, uint8_t val, uint32_t len) {
    uint8_t *temp = (uint8_t *)dest;
    for ( ; len != 0; len--) *temp++ = val;
}


void memcpy(void *_dst, const void *_src, uint32_t size) {
    uint8_t *dst = (uint8_t *)_dst;
    const uint8_t *src = (uint8_t *)_src;
    while (size--) {
         *dst++ = *src++;
    }
}

int memcmp(const void *_lhs, const void *_rhs, uint32_t size) {
    uint8_t *lhs = (uint8_t *)_lhs;
    uint8_t *rhs = (uint8_t *)_rhs;

    uint32_t i;
    for (i = 0; i < size; ++i) {
        if (*lhs++ != *rhs++) {
            return 1;
        }
    }
    return 0;
}
