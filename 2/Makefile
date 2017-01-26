CFLAGS     = -nostdlib -nostdinc -fno-builtin -fno-stack-protector -ggdb -Wall -O0 -Wno-main #-fstrength-reduce -fomit-frame-pointer -finline-functions -m32 -g
NASMFLAGS  = -felf
LDSCRIPT   = link.ld
LDFLAGS    = -T $(LDSCRIPT) -m elf_i386
OBJECTS    = kernel.o main.o monitor.o common.o apic.o
EXEC       = kernel


default: $(EXEC)


.c.o: 
	gcc -m32 $(CFLAGS) -c $< -o $@
	gcc -m32 $(CFLAGS) -S $<


.S.o:
	gcc -S $< > $<.s
	as --32 $<.s -o $@


$(EXEC): $(OBJECTS)
	ld $(LDFLAGS) -o $(EXEC) $(OBJECTS)


.PHONY: clean run


clean: 
	rm -f $(EXEC) *.o *.s


run: $(EXEC)
	qemu-system-i386 -kernel $(EXEC) -append "My kernel's command line."

run64: $(EXEC)
	qemu-system-x86_64 -kernel $(EXEC) -m 4G

debug: $(EXEC)
	qemu-system-i386 -kernel $(EXEC) -monitor stdio	-d int -singlestep

