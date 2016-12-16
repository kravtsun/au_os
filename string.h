#ifndef __STRING_H__
#define __STRING_H__

#include "stddef.h"
#include "defines.h"

size_t strlen(const char *str);
void *memcpy(void *dst, const void *src, size_t size);
void *memset(void *dst, int fill, size_t size);

char digit_to_char(int digit);
char char_to_digit(char c);

int str_compare(const char *a, const char *b, const size_t size);

#endif /*__STRING_H__*/
