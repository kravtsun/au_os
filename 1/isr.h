// isr.h -- Интерфейс и структуры для высокоуровневых подпрограмм обслуживания прерываний.
// ISR - Interrupt Service Routines.#ifndef ISR_H

#ifndef ISR_H
#define ISR_H

#include "common.h"


// Несколько определений, которые сделают жизнь немного проще.
#define IRQ0 32
#define IRQ1 33
#define IRQ2 34
#define IRQ3 35
#define IRQ4 36
#define IRQ5 37
#define IRQ6 38
#define IRQ7 39
#define IRQ8 40
#define IRQ9 41
#define IRQ10 42
#define IRQ11 43
#define IRQ12 44
#define IRQ13 45
#define IRQ14 46
#define IRQ15 47


typedef struct RegistersContainer
{
    uint32_t int_no, err_code;             // Номер прерывания и код ошибки (если он предоставляется)
    uint32_t eip, cs, eflags, useresp, ss; // Значения автоматически помещаются процессором в стек пре прерывании.
} RegistersContainer;


// Разрешает регистрировать обратные вызовы (callbacks) для прерываний или IRQ.
typedef void (*InterruptRoutine)();
void register_interrupt_handler(uint8_t n, InterruptRoutine handler);

#endif // ISR_H
