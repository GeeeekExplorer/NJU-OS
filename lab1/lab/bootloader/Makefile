bootloader.bin: start.s boot.c boot.h
	gcc -c -m32 start.s -o start.o
	gcc -c -m32 -O1 -fno-stack-protector boot.c -o boot.o
	ld -m elf_i386 -e start -Ttext 0x7c00 start.o boot.o -o bootloader.elf
	objcopy -S -j .text -O binary bootloader.elf bootloader.bin
	../utils/genboot.pl bootloader.bin

clean:
	rm -rf *.o *.elf *.bin
