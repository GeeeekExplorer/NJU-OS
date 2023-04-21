#include "x86.h"
#include "device.h"

#define TIMER_PORT 0x40
#define FREQ_8253 1193182
#define HZ 100
//#define HZ 1000

void initTimer() {
	int counter = FREQ_8253 / HZ;
	//assert(TIMER_PORT < 65536);
	outByte(TIMER_PORT + 3, 0x34);
	outByte(TIMER_PORT + 0, counter % 256);
	outByte(TIMER_PORT + 0, counter / 256);
}
