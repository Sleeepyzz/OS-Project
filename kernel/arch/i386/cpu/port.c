/*
port.c Basic in() and out() functions

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
