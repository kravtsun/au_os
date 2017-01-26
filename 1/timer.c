// timer.c -- Инициализирует PIT и обрабатывает обновления значений таймера.

#include "common.h"
#include "timer.h"
#include "isr.h"
#include "monitor.h"

#define PIT_BASE_FREQUENCY 1193180
// Значение, которое мы посылаем в PIT, является значением, на которое будет делиться основная частота
// (1193180 Hz) для того, чтобы получить необходимую нам частоту. Важно отметить, что делитель должен
// быть достаточно маленьким с тем, чтобы уместиться в 16 битов разрядной сетки.
#define PIT_BASE_ONE_SECOND_DIVISOR 32000

// PIT_BASE_ONE_SECOND_SECOND_DIVISOR = PIT_BASE_FREQUENCY / PIT_BASE_ONE_SECOND_DIVISOR
// 1193180 / 32000 ~= 37
#define TICK_RESET_VALUE 37

static void one_second_timer_callback() {
    static uint32_t tick = TICK_RESET_VALUE;

    if (tick == TICK_RESET_VALUE) {
        tick = 0;
        monitor_write("tick\n");
    }
    tick++;
}


void init_one_second_timer() {
    // Прежде всего регистрируем обратный вызов (callback) нашего таймера.
    register_interrupt_handler(IRQ0, &one_second_timer_callback);

    // Посылаем байт команды.
    outb(0x43, 0x36);

    // Делитель должен посылаться побайтно, так что мы делим его на старший и младший байты.
    uint8_t l = (uint8_t)(PIT_BASE_ONE_SECOND_DIVISOR      & 0xFF);
    uint8_t h = (uint8_t)((PIT_BASE_ONE_SECOND_DIVISOR>>8) & 0xFF);

    // Посылаем делитель частоты.
    outb(0x40, l);
    outb(0x40, h);
}
