#include <stdio.h>
#include <stdint.h>
#include <kernel/timer.h>
#include <kernel/irq.h>

#define PIT_NATURAL_FREQ 1193180
#define PIT_DATA0 0x40
#define PIT_DATA1 0x41
#define PIT_DATA2 0x42
#define PIT_COMMAND 0x43

uint32_t divisor;

struct regs
{
	unsigned int gs, fs, es, ds;
	unsigned int edi, esi, ebp, ebx, edx, ecx, eax;
	unsigned int int_no, err_code;
	unsigned int eip, cs, eflags, useresp, ss; 
};

volatile unsigned int t_ticks = 0;

void timer_handler(struct regs *r) {
	t_ticks++;
	outb(0x20, 0x20);
}

void timer_wait(int ticks)
{
    unsigned int eticks;
 
    eticks = t_ticks + ticks;
    while(t_ticks < eticks) 
    {
        __asm__ __volatile__ ("sti//hlt//cli");
    }
}

void timer_install(uint32_t frequency)
{

	divisor = PIT_NATURAL_FREQ / frequency;
	
	outb(PIT_COMMAND, 0x36);

	//Chop freq up into bytes and send to data0 port
	uint8_t low = (uint8_t)(divisor & 0xFF);
	uint8_t high = (uint8_t)((divisor >> 8) & 0xFF);

	outb(PIT_DATA0, low);
	outb(PIT_DATA0, high);
}
