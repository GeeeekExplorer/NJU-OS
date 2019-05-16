#bootloader.bin: start.S boot.c boot.h
#	gcc -c -m32 start.S -o start.o
#	gcc -c -m32 -O1 -fno-stack-protector boot.c -o boot.o
#	ld -m elf_i386 -e start -Ttext 0x7c00 start.o boot.o -o bootloader.elf
#	@#ld -m elf_i386 -e start -Ttext 0x7c00 boot.o start.o -o bootloader.elf
#	@#objcopy -S -j .text -O binary bootloader.elf bootloader.bin
#	objcopy -O binary bootloader.elf bootloader.bin
#	../utils/genBoot.pl bootloader.bin
#
#clean:
#	rm -rf *.o *.elf *.bin

# take care of link order of object files
# -Ttext set the address of the first byte of the text segment
# -e set the entry address in elf-header
# i.e., the entry address may not be the address of the first byte of the text segment

CC = gcc
LD = ld

CFLAGS = -m32 -march=i386 -static \
	 -fno-builtin -fno-stack-protector -fno-omit-frame-pointer \
	 -Wall -Werror -O2
ASFLAGS = -m32
LDFLAGS = -m elf_i386

BSFILES = $(shell find ./ -name "*.S")
BCFILES = $(shell find ./ -name "*.c")
BOBJS = $(BSFILES:.S=.o) $(BCFILES:.c=.o)

bootloader.bin: $(BOBJS)
	$(LD) $(LDFLAGS) -e start -Ttext 0x7c00 -o bootloader.elf $(BOBJS)
	objcopy -O binary bootloader.elf bootloader.bin
	@../utils/genBoot.pl bootloader.bin

clean:
	rm -rf $(BOBJS) bootloader.elf bootloader.bin
