#include "common.h"
#include "x86.h"
#include "device.h"

void kEntry(void) {

	// Interruption is disabled in bootloader

	initSerial();// initialize serial port
	initIdt(); // initialize idt
	initIntr(); // iniialize 8259a
	initSeg(); // initialize gdt, tss
	initVga(); // initialize vga device
	initTimer(); // initialize timer device
	initKeyTable(); // initialize keyboard device
	initSem(); // initialize semaphore list
	initDev(); // initialize device list
	initProc(); // initialize pcb & load user program
}
