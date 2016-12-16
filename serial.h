#ifndef __SERIAL_H__
#define __SERIAL_H__

#include "stddef.h"
#include "defines.h"

void serial_setup(void);
void serial_putchar(int c);
void serial_write(const char *buf, size_t size);
void serial_write_terminated(const char *buf);
void serial_write_hex(uint32_t num);

#endif /*__SERIAL_H__*/
