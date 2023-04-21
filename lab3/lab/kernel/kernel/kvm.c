#include "x86.h"
#include "device.h"

SegDesc gdt[NR_SEGMENTS];       // the new GDT, NR_SEGMENTS=10, defined in x86/memory.h
TSS tss;

ProcessTable pcb[MAX_PCB_NUM]; // pcb
int current; // current process

/*
MACRO
SEG(type, base, lim, dpl) (SegDesc) {...};
SEG_KCODE=1
SEG_KDATA=2
SEG_TSS=NR_SEGMENTS-1
DPL_KERN=0
DPL_USER=3
KSEL(desc) (((desc)<<3) | DPL_KERN)
USEL(desc) (((desc)<<3) | DPL_UERN)
asm [volatile] (AssemblerTemplate : OutputOperands [ : InputOperands [ : Clobbers ] ])
asm [volatile] (AssemblerTemplate : : InputOperands : Clobbers : GotoLabels)
*/
void initSeg() { // setup kernel segements
	int i;

	gdt[SEG_KCODE] = SEG(STA_X | STA_R, 0,       0xffffffff, DPL_KERN);
	gdt[SEG_KDATA] = SEG(STA_W,         0,       0xffffffff, DPL_KERN);

	for (i = 1; i < MAX_PCB_NUM; i++) {
		gdt[1+i*2] = SEG(STA_X | STA_R, (i+1)*0x100000,0x00100000, DPL_USER);
		gdt[2+i*2] = SEG(STA_W,         (i+1)*0x100000,0x00100000, DPL_USER);
	}
	
	gdt[SEG_TSS] = SEG16(STS_T32A,      &tss, sizeof(TSS)-1, DPL_KERN);
	gdt[SEG_TSS].s = 0;

	setGdt(gdt, sizeof(gdt)); // gdt is set in bootloader, here reset gdt in kernel

	/* initialize TSS */
	tss.ss0 = KSEL(SEG_KDATA);
	asm volatile("ltr %%ax":: "a" (KSEL(SEG_TSS)));

	/* reassign segment register */
	asm volatile("movw %%ax,%%ds":: "a" (KSEL(SEG_KDATA)));
	asm volatile("movw %%ax,%%ss":: "a" (KSEL(SEG_KDATA)));

	lLdt(0);
	
}

uint32_t loadUMain(void);

void initProc() {
	int i;
	for (i = 0; i < MAX_PCB_NUM; i++) {
		pcb[i].state = STATE_DEAD;
	}
	// kernel process
	pcb[0].stackTop = (uint32_t)&(pcb[0].stackTop);
	pcb[0].prevStackTop = (uint32_t)&(pcb[0].stackTop);
	pcb[0].state = STATE_RUNNING;
	pcb[0].timeCount = MAX_TIME_COUNT;
	pcb[0].sleepTime = 0;
	pcb[0].pid = 0;
	// user process
	pcb[1].stackTop = (uint32_t)&(pcb[1].regs);
	pcb[1].prevStackTop = (uint32_t)&(pcb[1].stackTop);
	pcb[1].state = STATE_RUNNABLE;
	pcb[1].timeCount = 0;
	pcb[1].sleepTime = 0;
	pcb[1].pid = 1;
	pcb[1].regs.ss = USEL(4);
	pcb[1].regs.esp = 0x100000;
	asm volatile("pushfl");
	asm volatile("popl %0":"=r"(pcb[1].regs.eflags));
	pcb[1].regs.eflags = pcb[1].regs.eflags | 0x200;
	pcb[1].regs.cs = USEL(3);
	pcb[1].regs.eip = loadUMain();
	pcb[1].regs.ds = USEL(4);
	pcb[1].regs.es = USEL(4);
	pcb[1].regs.fs = USEL(4);
	pcb[1].regs.gs = USEL(4);

	current = 0; // kernel idle process
	asm volatile("movl %0, %%esp"::"m"(pcb[0].stackTop)); // switch to kernel stack for kernel idle process
	enableInterrupt();
	asm volatile("int $0x20"); // trigger irqTimer
	while(1)
		waitForInterrupt();
}

/*
kernel is loaded to location 0x100000, i.e., 1MB
size of kernel is not greater than 200*512 bytes, i.e., 100KB
user program is loaded to location 0x200000, i.e., 2MB
size of user program is not greater than 200*512 bytes, i.e., 100KB
*/

uint32_t loadUMain(void) {
	int i = 0;
	int phoff = 0x34; // program header offset
	int offset = 0x1000; // .text section offset
	uint32_t elf = 0x200000; // physical memory addr to load
	uint32_t uMainEntry = 0x200000;

	for (i = 0; i < 200; i++) {
		readSect((void*)(elf + i*512), 201+i);
	}
	
	uMainEntry = ((struct ELFHeader *)elf)->entry; // entry address of the program
	phoff = ((struct ELFHeader *)elf)->phoff;
	offset = ((struct ProgramHeader *)(elf + phoff))->off;

	for (i = 0; i < 200 * 512; i++) {
		*(uint8_t *)(elf + i) = *(uint8_t *)(elf + i + offset);
	}

	return uMainEntry;
}
