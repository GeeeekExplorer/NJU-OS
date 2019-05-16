CC = gcc
LD = ld

CFLAGS = -m32 -march=i386 -static \
	 -fno-builtin -fno-stack-protector -fno-omit-frame-pointer \
	 -Wall -Werror -O0 -I../lib
LDFLAGS = -m elf_i386

UCFILES = $(shell find ./ -name "*.c")
LCFILES = $(shell find ../lib -name "*.c")
UOBJS = $(UCFILES:.c=.o) $(LCFILES:.c=.o)
#UOBJS = $(LCFILES:.c=.o) $(UCFILES:.c=.o)

umain.bin: $(UOBJS)
	@#$(LD) $(LDFLAGS) -e uEntry -Ttext 0x00200000 -o uMain.elf $(UOBJS)
	$(LD) $(LDFLAGS) -e uEntry -Ttext 0x00000000 -o uMain.elf $(UOBJS)
	@#objcopy -S -j .text -j .rodata -j .eh_frame -j .data -j .bss -O binary uMain.elf uMain.bin
	@#objcopy -O binary uMain.elf uMain.bin

clean:
	@#rm -rf $(UOBJS) uMain.elf uMain.bin
	rm -rf $(UOBJS) uMain.elf
