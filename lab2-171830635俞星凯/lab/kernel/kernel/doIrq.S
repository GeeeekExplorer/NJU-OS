/*TODO
otherwise need to reassign esp0 of tss in task switching for each user process
note that for conforming code OR same PRL, no need to switch task
that is TSS doesn't work
1. Trap Gate, IF won't be set
2. Interrupt Gate, IF is set automatically
3. System Gate
*/

.code32

.global irqEmpty
irqEmpty:
	pushl $0 // push dummy error code
	pushl $-1 // push interruption number into kernel
	jmp asmDoIrq

.global irqErrorCode
irqErrorCode:
	pushl $-1 // push interruption number into kernel
	jmp asmDoIrq

.global irqDoubleFault
irqDoubleFault:
	pushl $-1
	jmp asmDoIrq

.global irqInvalidTSS
irqInvalidTSS:
	pushl $-1
	jmp asmDoIrq

.global irqSegNotPresent
irqSegNotPresent:
	pushl $-1
	jmp asmDoIrq

.global irqStackSegFault
irqStackSegFault:
	pushl $-1
	jmp asmDoIrq

.global irqGProtectFault
irqGProtectFault:
	pushl $0xd
	jmp asmDoIrq

.global irqPageFault
irqPageFault:
	pushl $-1
	jmp asmDoIrq

.global irqAlignCheck
irqAlignCheck:
	pushl $-1
	jmp asmDoIrq

.global irqSecException
irqSecException:
	pushl $-1
	jmp asmDoIrq

.global irqSyscall
irqSyscall:
	pushl $0 // push dummy error code
	pushl $0x80 // push interruption number into kernel stack
	jmp asmDoIrq

//.extern irqHandle //defined in irq_handle.c

.global asmDoIrq
asmDoIrq:
	pushal // push process state into kernel stack
	pushl %esp //esp is treated as a parameter
	call irqHandle
	addl $4, %esp //esp is on top of kernel stack
	popal
	addl $4, %esp //interrupt number is on top of kernel stack
	addl $4, %esp //error code is on top of kernel stack
	iret
