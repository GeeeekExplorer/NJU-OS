os.img:
	cd bootloader; make bootloader.bin
	cd app; make app.bin
	cat bootloader/bootloader.bin app/app.bin > os.img

clean:
	cd bootloader; make clean
	cd app; make clean
	rm -f os.img

play:
	qemu-system-i386 os.img

debug:
	qemu-system-i386 -s -S os.img
