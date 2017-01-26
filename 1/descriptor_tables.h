#ifndef DESCRIPTOR_TABLES_H
#define DESCRIPTOR_TABLES_H

#include "common.h"


// Эта функция должна быть доступна глобально.
void init_descriptor_tables();


// В этой структуре хранится содержимое одной записи GDT.
// Мы используем атрибут 'packed', которые указывает компилятору GCC,
// что в этой структуре выравнивание не выполняется.
typedef struct GDTDescriptor
{
    uint16_t limit_low;        // Младшие 16 битов граничного значения limit.
    uint16_t base_low;         // Младшие 16 битов адресной базы.
    uint8_t  base_middle;      // Следующие 8 битов адресной базы.
    uint8_t  access;           // Флаги доступа, определяющие в каком кольце можно использовать этот сегмент.
    uint8_t  granularity;
    uint8_t  base_high;        // Последние 8 битов адресной базы.
} __attribute__((packed)) GDTDescriptor;


// This struct describes a GDT pointer. It points to the start of
// our array of GDT entries, and is in the format required by the
// lgdt instruction.
typedef struct GDTPointer
{
    uint16_t size;             // Верхние 16 битов всех предельных значений селектора.
    uint32_t base;             // Адрес первой структуры gdt_entry_t.
} __attribute__((packed)) GDTPointer;


// Структура, описывающая шлюз прерываний.
typedef struct IDTDescriptor
{
    uint16_t offset_1;         // Младшие 16 битов адреса, куда происходит переход в случае возникновения прерывания.
    uint16_t selector;         // Переключатель сегмента ядра.
    uint8_t  zero;             // Это значение всегда должно быть нулевым.
    uint8_t  type_attr;        // More flags. See documentation.
    uint16_t offset_2;         // Старшие 16 битов адреса, куда происходит переход.
} __attribute__((packed)) IDTDescriptor;


// Структура, описывающая указатель на массив обработчиков прерываний.
// Этот формат подходит для загрузки с помощью 'lidt'.
typedef struct IDTPointer
{
    uint16_t size;
    uint32_t base;                // Адрес первого элемента нашего массива idt_entry_t.
} __attribute__((packed)) IDTPointer;


// Эти внешние директивы позволят нам получить доступ к адресам наших ассемблерных обработчиков прерываний ISR.
extern void isr0 ();
extern void isr1 ();
extern void isr2 ();
extern void isr3 ();
extern void isr4 ();
extern void isr5 ();
extern void isr6 ();
extern void isr7 ();
extern void isr8 ();
extern void isr9 ();
extern void isr10();
extern void isr11();
extern void isr12();
extern void isr13();
extern void isr14();
extern void isr15();
extern void isr16();
extern void isr17();
extern void isr18();
extern void isr19();
extern void isr20();
extern void isr21();
extern void isr22();
extern void isr23();
extern void isr24();
extern void isr25();
extern void isr26();
extern void isr27();
extern void isr28();
extern void isr29();
extern void isr30();
extern void isr31();
extern void irq0 ();
extern void irq1 ();
extern void irq2 ();
extern void irq3 ();
extern void irq4 ();
extern void irq5 ();
extern void irq6 ();
extern void irq7 ();
extern void irq8 ();
extern void irq9 ();
extern void irq10();
extern void irq11();
extern void irq12();
extern void irq13();
extern void irq14();
extern void irq15();


#endif // DESCRIPTOR_TABLES_H
