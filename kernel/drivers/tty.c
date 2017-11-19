/*
tty.c The terminal driver

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

#include <stddef.h>
#include <stdint.h>
#include <string.h>

#include <kernel/tty.h>
#include <kernel/vga.h>
#include <kernel/cursor.h>
#include <kernel/shell.h>

static const size_t VGA_WIDTH = 80;
static const size_t VGA_HEIGHT = 25;
static uint16_t* const VGA_MEMORY = (uint16_t*) 0xC00B8000;

size_t terminal_row;
size_t terminal_column;
uint8_t terminal_color;
static uint16_t* terminal_buffer;

void terminal_initialize(void) {
	terminal_row = 0;
	terminal_column = 0;
	terminal_color = vga_entry_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);
	terminal_buffer = VGA_MEMORY;
	for (size_t y = 0; y < VGA_HEIGHT; y++) {
		for (size_t x = 0; x < VGA_WIDTH; x++) {
			const size_t index = y * VGA_WIDTH + x;
			terminal_buffer[index] = vga_entry(' ', terminal_color);
		}
	}
}

void terminal_setcolor(uint8_t color) {
	terminal_color = color;
	if (shell_init) terminals[termNum].color = color;
}

void terminal_putentryat(unsigned char c, uint8_t color, size_t x, size_t y) {
	const size_t index = y * VGA_WIDTH + x;
	terminal_buffer[index] = vga_entry(c, color);
	if (shell_init) terminals[termNum].buffer[index] = vga_entry(c, color);
}

void terminal_clear() {
	for (size_t y = 0; y < VGA_HEIGHT; y++) {
		for (size_t x = 0; x < VGA_WIDTH; x++) {
			const size_t index = y * VGA_WIDTH + x;
			terminal_buffer[index] = vga_entry(' ', terminal_color);
			if (shell_init) terminals[termNum].buffer[index] = vga_entry(' ', terminal_color);
		}
	}
	if (shell_init) {
		terminals[termNum].x = 0;
		terminals[termNum].y = 0;
	}
	terminal_row = 0;
	terminal_column = 0;
}

void clearRow(int number) {
	for (size_t x = 0; x < VGA_WIDTH; x++) {
		const size_t index = number * VGA_WIDTH + x;
		terminal_buffer[index] = vga_entry(' ', terminal_color);
		if (shell_init) terminals[termNum].buffer[index] = vga_entry(' ', terminal_color);
	}
}

void set_buffer(uint16_t* buffer) {
	memcpy(terminal_buffer, buffer, VGA_WIDTH * VGA_HEIGHT * 2);
}

void scroll(void)
{
    unsigned blank, temp;

    /* A blank is defined as a space... we need to give it
    *  backcolor too */
    blank = 0x20 | (0x0F << 8);

    /* Row 25 is the end, this means we need to scroll up */
    if(terminal_row >= 23)
    {
        /* Move the current text chunk that makes up the screen
        *  back in the buffer by a line */
        temp = terminal_row - 25 + 3;
        memcpy (terminal_buffer, terminal_buffer + temp * 80, (25 - temp) * 80 * 2);
	if (shell_init) {
		memcpy(terminals[termNum].buffer, terminals[termNum].buffer + temp * 80, (25 - temp) * 80 * 2);
	}

        /* Finally, we set the chunk of memory that occupies
        *  the last line of text to our 'blank' character */
        memsetw (terminal_buffer + (25 - temp) * 80, blank, 80);
	if (shell_init) {
		memsetw(terminals[termNum].buffer + (25 - temp) * 80, blank, 80);
	}
        terminal_row = 25 - 3;
    }
}

void terminal_putchar(char c) {
	if (shell_init) {
		terminal_column = terminals[termNum].x;
		terminal_row = terminals[termNum].y;
	}
	unsigned char uc = c;
	unsigned short pos;
	switch (uc)
	{
	case '\n':
		terminal_column = 0;
		terminal_row++;
		scroll();
		for (size_t x = terminal_column; x < VGA_WIDTH; x++) {
			const size_t index = terminal_row * VGA_WIDTH + x;
			if (shell_init) {
				terminals[termNum].buffer[index] = vga_entry(' ', terminal_color);
			}
			terminal_buffer[index] = vga_entry(' ', terminal_color);
		}
		move_cursor((terminal_row * 80) + terminal_column);
		break;
	case '\b':
		if(terminal_column > 0) {	
            		terminal_putentryat(' ', terminal_color, terminal_column, terminal_row);
			terminal_column--;
        	}
		else {
			terminal_row--;
			terminal_column = 78;
			terminal_putentryat(' ', terminal_color, terminal_column, terminal_row);
			terminal_column--;
			
		}
		pos = (terminal_row * 80) + terminal_column;
		move_cursor(pos + 1);
		break;
	default:
		if (++terminal_column == VGA_WIDTH) {
			terminal_column = 0;
			if (++terminal_row == VGA_HEIGHT) {
				terminal_row = 0;
			}
		}
		if (terminal_column == VGA_WIDTH) {
			terminal_column = 0;
			terminal_row++;
		}
		terminal_putentryat(uc, terminal_color, terminal_column, terminal_row);
		pos = (terminal_row * 80) + terminal_column;
		move_cursor(pos + 1);
	}
	scroll();
	if (shell_init) {
		terminals[termNum].x = terminal_column;
		terminals[termNum].y = terminal_row;
	}
}

void terminal_write(const char* data, size_t size) {
	for (size_t i = 0; i < size; i++)
		terminal_putchar(data[i]);
}

void terminal_writestring(const char* data) {
	terminal_write(data, strlen(data));
}
