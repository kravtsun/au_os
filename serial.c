#include "serial.h"
#include "ioport.h"
#include "string.h"

#define SERIAL_BASE_PORT	0x3f8
#define SERIAL_PORT(x)		(SERIAL_BASE_PORT + (x))
#define REG_DATA		SERIAL_PORT(0)
#define REG_DLL			SERIAL_PORT(0)
#define REG_IER			SERIAL_PORT(1)
#define REG_DLH			SERIAL_PORT(1)
#define REG_LCR			SERIAL_PORT(3)
#define REG_LSR			SERIAL_PORT(5)

#define LCR_8BIT		(3 << 0)
#define LCR_DLAB		(1 << 7)
#define LSR_TX_READY		(1 << 5)


void serial_setup(void)
{
	/* Step by step explanation:
	 *   1. i want to disable interrupt, in order to do that i need access
	 *      to the IER, before writing IER we need to drop DLAB bit in the
	 *      LCR, so at first i write 0 to LCR, and then write 0 to IER; */
	out8(REG_LCR, 0);
	out8(REG_IER, 0);
	/*   2. now i want to set speed, let say, to 9600 baud,
	 *      115200/9600 = 12 - is our divisor, but before setting divisor,
	 *      we need to set DLAB bit in the LCR; */
	out8(REG_LCR, LCR_DLAB);
	out8(REG_DLL, 0x0C);
	out8(REG_DLH, 0x00);
	/*   3. finally i want to set frame format, there is no tricky parts
	 *      here. */
	out8(REG_LCR, LCR_8BIT);
}

void serial_putchar(int c)
{
	while (!(in8(REG_LSR) & LSR_TX_READY));
	out8(REG_DATA, c);
}

void serial_write(const char *buf, size_t size)
{
	for (size_t i = 0; i != size; ++i)
		serial_putchar(buf[i]);
}

void serial_write_terminated(const char *buf)
{
    const size_t n = strlen(buf);
    for (size_t i = 0; i < n; ++i)
    {
        serial_putchar(buf[i]);
    }
}

#define WRITE_HEX_BUF_SIZE 8
void serial_write_hex(uint32_t num)
{
    static char buf[WRITE_HEX_BUF_SIZE];
    const size_t buf_size = WRITE_HEX_BUF_SIZE;
    memset(buf, '0', buf_size);

    for (int i = buf_size-1; i >= 0 && num; --i, num /= 16)
    {
        buf[i] = digit_to_char(num % 16);
    }
    serial_putchar('0');
    serial_putchar('x');
    serial_write(buf, buf_size+1);
    serial_putchar('\n');
}
