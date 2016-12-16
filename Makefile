#CFLAGS     = -m64 -nostdlib -nostdinc -fno-builtin -fno-stack-protector -ggdb -Wall -O0 -Wno-main 
#-fstrength-reduce -fomit-frame-pointer -finline-functions -m64 -g
CFLAGS := -g -m64 -mno-red-zone -mno-mmx -mno-sse -mno-sse2 -ffreestanding \
	-mcmodel=kernel -Wall -Wextra -Werror -pedantic -std=c99 \
	-Wframe-larger-than=1024 -Wstack-usage=1024 \
	-MMD -Wno-unknown-warning-option
	
LDSCRIPT   = kernel.ld
LDFLAGS    = -T $(LDSCRIPT) -s -nostdlib -nostdinc -z max-page-size=0x1000
SRCDIR     = ./
EXEC       = kernel

CSOURCES = $(wildcard $(SRCDIR)/*.c)
COBJECTS = $(CSOURCES:.c=.o)
SSOURCES = $(wildcard $(SRCDIR)/*.S)
SOBJECTS = $(SSOURCES:.S=.o)

OBJECTS  = $(COBJECTS) $(SOBJECTS)


default: $(EXEC)


.c.o: 
	gcc $(CFLAGS) -c $< -o $@


.S.o:
	gcc -D__ASM_FILE__ -I. -g -MMD -c $< -o $@


$(EXEC): $(OBJECTS)
	ld $(LDFLAGS) -o $(EXEC) $(OBJECTS)


.PHONY: clean run


clean:
	rm -f $(EXEC) *.o *.s *.d


run: $(EXEC)
	qemu-system-x86_64 -kernel $(EXEC) -serial stdio -initrd misc/initramfs_image.cpio

debug: $(EXEC)
	qemu-system-x86_64 -kernel $(EXEC) -monitor stdio -d int -singlestep -serial stdio  -initrd misc/initramfs_image.cpio

