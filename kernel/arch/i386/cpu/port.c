#include <kernel/port.h>

void outb(uint16_t port, uint8_t data) {
	asm volatile ("outb %1, %0" : : "dN" (port), "a" (data));
}

void outw(uint16_t port, uint16_t val)
{
    	asm volatile("outw %%ax, %%dx": :"d" (port), "a" (val));
}

uint8_t inb(uint16_t port) {
	uint8_t rv;
	__asm__ __volatile__ ("inb %1, %0" : "=a" (rv) : "dN" (port));
	return rv;
}
