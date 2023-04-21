#include "x86.h"

#define PORT_PIC_MASTER 0x20
#define PORT_PIC_SLAVE  0xA0
#define IRQ_SLAVE       2

/* 初始化8259中断控制器：
 * 硬件中断IRQ从32号开始，自动发送EOI */
void
initIntr(void) {
	//outByte(PORT_PIC_MASTER + 1, 0xFF); // OCW1, Disable Master PIC all IRQs
	//outByte(PORT_PIC_SLAVE + 1 , 0xFF); // OCW1, Disable Slave PIC all IRQs
	outByte(PORT_PIC_MASTER, 0x11); // ICW1, Initialization command
	outByte(PORT_PIC_SLAVE, 0x11); // ICW1, Initialization command
	outByte(PORT_PIC_MASTER + 1, 32); // ICW2, Interrupt Vector Offset 0x20
	outByte(PORT_PIC_SLAVE + 1, 32 + 8); // ICW2, Interrupt Vector Offset 0x28
	outByte(PORT_PIC_MASTER + 1, 1 << 2); // ICW3, Tell Master PIC that there is a slave
	outByte(PORT_PIC_SLAVE + 1, 2); // ICW3, Tell Slave PIC its cascade identity
	outByte(PORT_PIC_MASTER + 1, 0x3); // ICW4, Auto EOI in 8086/88 mode
	outByte(PORT_PIC_SLAVE + 1, 0x3); // ICW4, Auto EOI in 8086/88 mode

	//outByte(PORT_PIC_MASTER, 0x68); // OCW3, Enable Special Mask Mode
	//outByte(PORT_PIC_MASTER, 0x0A); // OCW3, Read ISR of Master on next CMD Read 
	//outByte(PORT_PIC_SLAVE, 0x68); // OCW3, Enable Special Mask Mode
	//outByte(PORT_PIC_SLAVE, 0x0A); // OCW3, Read ISR of Slave on next CMD Read

	//outByte(PORT_PIC_MASTER + 1, 0xFE); // OCW1, Enable Timer IRQ
	//outByte(PORT_PIC_MASTER + 1, 0xFF); // OCW1, Disable Master PIC all IRQs
	//outByte(PORT_PIC_SLAVE + 1, 0xFF); // OCW1, Disable Slave PIC all IRQs
}
