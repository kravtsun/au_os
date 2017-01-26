// common.h -- Defines typedefs and some global functions.

#ifndef COMMON_H
#define COMMON_H

typedef unsigned int   uint32_t;
typedef          int   int32_t;
typedef unsigned short uint16_t;
typedef          short int16_t;
typedef unsigned char  uint8_t;
typedef          char  int8_t;

// Write a byte out to the specified port.
void outb(uint16_t port, uint8_t value);

// Read a byte from the specified port.
uint8_t inb(uint16_t port);

// Read a word from the specified port.
uint16_t inw(uint16_t port);

// Write len copies of val into dest.
void memset(uint8_t *dest, uint8_t val, uint32_t len);

#endif // COMMON_H
