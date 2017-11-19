/*
keyboard.c The kernel keyboard driver

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

#include <stdint.h>
#include <kernel/idt.h>
#include <kernel/keyboard.h>
#include <kernel/port.h>
#include <kernel/mm.h>
#include <kernel/shell.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

char* buffstr;
int reading = 0;

int count; 
uint8_t i;
uint8_t shift_on;
uint8_t caps_lock;
int thing;	

void keyboard_init() {
	buffstr = (char*)kmalloc(200);
}

char* readStr()
{
    	count = commands_entered - 1;
    	i = 0;
    	thing = 0;
	shift_on = 0;
    	caps_lock = 0;
    	reading = 1;
    	for (int n = 0; n < 200; n++) {
		buffstr[n] = 0;
    	}
	while(reading) {
		printf("");
	}
	return buffstr;
}

static char* _numbers = "1234567890-=";
static char* special_char = "!@#$%^&*()_+";
static char* _letters0 = "qwertyuiop[]";
static char* _letters1 = "asdfghjkl;'`";
static char* _letters2 = "zxcvbnm,./";


void keyboard_handler(struct regs *r) {
	unsigned char scancode;

    	scancode = inb(0x60);
	
	if (scancode == 0xE) {
		buffstr[--i] = ' ';
		putchar('\b');
	}
   	else if (scancode == 0x1C) reading = 0;
	else if (scancode == 0x2A) shift_on = 1;
	else if (scancode == 0x36) shift_on = 1;
	else if (scancode == 0x39) {
		putchar(' ');
		buffstr[i++] = ' ';
	}
	else {
		if (shift_on == 1) {
			if (scancode > 0x1 && scancode < 0xE) {
				putchar(special_char[scancode - 0x2]);
				buffstr[i++] = special_char[scancode - 0x2];
				shift_on = 0;
			}
		}
		else {
			if (scancode > 0x1 && scancode < 0xE) {
				putchar(_numbers[scancode - 0x2]);
				buffstr[i++] = _numbers[scancode - 0x2];
				shift_on = 0;
			}
		}
		if (scancode > 0xF && scancode < 0x1C) {
			putchar(_letters0[scancode - 0x10]);
			buffstr[i++] = _letters0[scancode - 0x10];
			shift_on = 0;
		}
		if (scancode > 0x1D && scancode < 0x2A){
			putchar(_letters1[scancode - 0x1E]);
			buffstr[i++] = _letters1[scancode - 0x1E];
			shift_on = 0;
		}
		if (scancode > 0x2B && scancode < 0x36) {
			putchar(_letters2[scancode - 0x2C]);
			buffstr[i++] = _letters2[scancode - 0x2C];
			shift_on = 0;
		}
	}
	outb(0x20,0x20);
}
