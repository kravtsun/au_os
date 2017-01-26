CFLAGS     = -nostdlib -nostdinc -fno-builtin -fno-stack-protector -ggdb -Wall
NASMFLAGS  = -felf
LDSCRIPT   = link.ld
LDFLAGS    = -T $(LDSCRIPT) -m elf_i386
OBJECTS    = kernel.o main.o monitor.o common.o descriptor_tables.o isr.o interrupt.o gdt.o timer.o
EXEC       = ticker


default: $(EXEC)


.c.o: 
	gcc -m32 $(CFLAGS) -c $< -o $@


.S.o:
	as --32 $< -o $@


$(EXEC): $(OBJECTS)
	ld $(LDFLAGS) -o $(EXEC) $(OBJECTS)


.PHONY: clean run


clean: 
	rm -f $(EXEC) *.o kernel hello


run: $(EXEC)
	qemu-system-i386 -kernel $(EXEC)


debug: $(EXEC)
	qemu-system-i386 -s -S -kernel $(EXEC) -monitor stdio	

