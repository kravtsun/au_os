#include "string.h"

size_t strlen(const char *str)
{
	const char *begin = str;

	while (*str++);
	return str - begin - 1;
}

void *memcpy(void *dst, const void *src, size_t size)
{
	char *to = dst;
	const char *from = src;

	while (size--)
		*to++ = *from++;
	return dst;
}

void *memset(void *dst, int fill, size_t size)
{
	char *to = dst;

	while (size--)
		*to++ = fill;
	return dst;
}

char digit_to_char(int digit)
{
    if (digit < 10) return digit + '0';
    return 'A' + (digit - 10);
}

char char_to_digit(char c)
{
    if (c <= '9' && c >= '0')
    {
        return c - '0';
    }
    else if (c >= 'A' && c <= 'F')
    {
        return 10 + (c - 'A');
    }
    else
    {
        return 10 + (c - 'a');
    }
}


//uint32_t hex_to_dec(uint32_t hex)
//{
//    uint32_t res = 0;
//    uint32_t mul = 1;
//    while (hex)
//    {
//        res += mul * (hex % 16);
//        mul *= 16;
//    }
//    return res;
//}

#define MIN(a, b) (a) < (b)? (a) : (b)
#define ABS(a) (a) < 0? -(a) : (a)

uint32_t atoi(const char *buf, size_t size, uint32_t base)
{
    uint32_t mul = 1, res = 0;
    for (size_t i = 0; i < size; ++i)
    {
        char c = buf[size-i-1];
        uint32_t digit = c - '0';
        if (base > 10)
        {
            digit = MIN(ABS(c - 'A'), ABS(c - 'a'));
        }
        res += mul * digit;
        mul *= base;
    }
    return res;
}

int str_compare(const char *a, const char *b, const size_t size)
{
    for (size_t i = 0; i < size; ++i)
    {
        if (a[i] < b[i])
        {
            return -1;
        }
        else if (a[i] > b[i])
        {
            return 1;
        }
    }
    return 0;
}
