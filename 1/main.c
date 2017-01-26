// main.c -- Kernel entry point, monitor initialization routines.

#include "monitor.h"
#include "descriptor_tables.h"
#include "timer.h"


// int main(struct multiboot *mboot_ptr)
int main() {
    // Initialize all the ISRs and segmentation
    init_descriptor_tables();
    // Initialize the screen (by clearing it)
    monitor_clear();

    init_one_second_timer();
    asm volatile("sti");

    return 0;
}
