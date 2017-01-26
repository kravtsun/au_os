// monitor.h -- Defines the interface for monitor.h

#ifndef MONITOR_H
#define MONITOR_H


#include "common.h"


// Writes a single character out to the screen.
void monitor_put(char c);


// Clears the screen, by copying lots of spaces to the framebuffer.
void monitor_clear();


// Write a null-terminated ASCII string to the monitor.
void monitor_write(const char *c);


void monitor_write_common_number(uint32_t num, uint32_t base);

static inline void monitor_write_number(uint32_t num) {
    monitor_write_common_number(num, 16);
}

static inline void monitor_write_decimal_number(uint32_t num) {
    monitor_write_common_number(num, 10);
}

void monitor_debug(const char *str, uint32_t num);

void monitor_write_big_number(uint64_t num);


static inline void print_registers(RegistersContainer regs) {
    monitor_write("regs.int_no   = ");   monitor_write_number(regs.int_no);
    monitor_write("regs.edi      = ");   monitor_write_number(regs.edi);
    monitor_write("regs.esi      = ");   monitor_write_number(regs.esi);
    monitor_write("regs.ebp      = ");   monitor_write_number(regs.ebp);
    monitor_write("regs.esp      = ");   monitor_write_number(regs.esp);
    monitor_write("regs.ebx      = ");   monitor_write_number(regs.ebx);
    monitor_write("regs.edx      = ");   monitor_write_number(regs.edx);
    monitor_write("regs.ecx      = ");   monitor_write_number(regs.ecx);
    monitor_write("regs.eax      = ");   monitor_write_number(regs.eax);
    monitor_write("regs.int_no   = ");   monitor_write_number(regs.int_no);
    monitor_write("regs.err_code = ");   monitor_write_number(regs.err_code);
    monitor_write("regs.eip      = ");   monitor_write_number(regs.eip);
    monitor_write("regs.cs       = ");   monitor_write_number(regs.cs);
    monitor_write("regs.eflags   = ");   monitor_write_number(regs.eflags);

}



static inline void assert(int condition, const char *str) {
    if (!condition) {
        if (!str) {
            monitor_write("Something bad happened: ");
        }
        else {
            monitor_write(str);
        }
        __asm__ __volatile__ ("hlt");
    }
}


#endif // MONITOR_H
