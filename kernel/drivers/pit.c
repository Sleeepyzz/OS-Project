/*
pit.c Code for the programmable interval timer

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

#include <stdio.h>
#include <stdint.h>
#include <kernel/timer.h>


uint32_t divisor;

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
