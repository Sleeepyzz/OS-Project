/*
irq.c Interrupt Request Handler

Copyright (C) 2017 Ryken Thompson

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program. If not, see <http://www.gnu.org/licenses/>.

*/

#include <kernel/idt.h>
#include <kernel/port.h>
#include <stdio.h>
#include <stdint.h>

static inline void io_wait(void)
{
    asm volatile ( "jmp 1f\n\t"
                   "1:jmp 2f\n\t"
                   "2:" );
}

isr_t irqs[16];

void irq_remap() {
	unsigned char a1, a2;
	
    	outb(0x20, 0x11);
    	outb(0xA0, 0x11);
    	outb(0x21, 0x20);
    	outb(0xA1, 0x28);
    	outb(0x21, 0x04);
    	outb(0xA1, 0x02);
    	outb(0x21, 0x01);
    	outb(0xA1, 0x01);
    	outb(0x21, 0x0);
	outb(0xA1, 0x0);
}

void irq_install_handler(uint8_t irq, isr_t handler) {
	irqs[irq] = handler;
}

void irq_uninstall_handler(int irq) {
	irqs[irq] = 0;
}

void irq_handler(struct regs *r) {

	if (irqs[r->int_no] != 0) {
		isr_t handler = irqs[r->int_no - 32];
		handler(r);
	}
	
	if (r->int_no >= 40) {
		outb(0xA0, 0x20);
	}

	outb(0x20, 0x20);
	asm ("hlt");
}
