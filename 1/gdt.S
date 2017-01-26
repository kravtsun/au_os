# gdt.S -- contains global descriptor table and interrupt descriptor table setup code.

.globl gdt_flush     # Позволяет коду на C обращаться к gdt_flush().

gdt_flush:
   mov 4(%esp), %eax  # Берет указатель на таблицу GDT, переданную в качестве параметра.
   lgdt (%eax)        # Загружает новый указатель GDT.

   movl $0x10, %eax   # 0x10 является смещением, находящимся в таблице GDT и указываемым на наш сегмент данных.
   movl %eax, %ds     # Загрузка переключателей всех сегментов данных.
   movl %eax, %ss
   ljmp $0x08,$.flush # 0x08 является смещением (в записи GDT) на текущий сегмент кода.
.flush:
   ret

.globl idt_flush      # Позволяет коду на С обращаться к idt_flush().

idt_flush:
   mov 4(%esp), %eax  # Берет указатель на IDT, передаваемый в качестве параметра.
   lidt (%eax)        # Загружает указатель IDT.
   ret
