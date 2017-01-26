// isr.c -- Высокоуровневые подпрограмма обслуживания прерываний и обработчики запросов прерываний.

#include "common.h"
#include "isr.h"
#include "monitor.h"

static InterruptRoutine interrupt_handlers[256];


void register_interrupt_handler(uint8_t n, InterruptRoutine handler) {
    interrupt_handlers[n] = handler;
}


// Сюда поступает вызов из обработчика прерываний, написанного на ассемблере.
void isr_handler(RegistersContainer regs) {
    if (interrupt_handlers[regs.int_no] != 0) {
        InterruptRoutine handler = interrupt_handlers[regs.int_no];
        handler(regs);
    }
}


// Этот фрагмент вызывается из кода обработчика прерывания, написанного на ассемблере.
void irq_handler(RegistersContainer regs) {
    // Посылает сигнал EOI (завершение прерывания) в устройства PIC,
    // если к возникновению прерывания причастно подчиненное устройство.
    if (regs.int_no >= 40) {
        // Посылаем сигнал перезагрузки подчиненному устройству.
        outb(0xA0, 0x20);
    }
    // Посылает сигнал перезагрузки в главное устройство
    // (а также в подчиненное устройство, если это необходимо).
    outb(0x20, 0x20);

    if (interrupt_handlers[regs.int_no] != 0) {
        InterruptRoutine handler = interrupt_handlers[regs.int_no];
        handler(regs);
    }
}
