#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>

#include <kernel/tty.h>

#include "vga.h"
#include <kernel/cursor.h>

static const size_t VGA_WIDTH = 80;
static const size_t VGA_HEIGHT = 25;
static uint16_t* const VGA_MEMORY = (uint16_t*) 0xB8000;

size_t terminal_row;
size_t terminal_column;
static uint8_t terminal_color;
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
}

void terminal_putentryat(unsigned char c, uint8_t color, size_t x, size_t y) {
	const size_t index = y * VGA_WIDTH + x;
	terminal_buffer[index] = vga_entry(c, color);
}

void terminal_clear() {
	for (size_t y = 0; y < VGA_HEIGHT; y++) {
		for (size_t x = 0; x < VGA_WIDTH; x++) {
			const size_t index = y * VGA_WIDTH + x;
			terminal_buffer[index] = vga_entry(' ', terminal_color);
		}
	}
	terminal_row = 0;
	terminal_column = 0;
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

        /* Finally, we set the chunk of memory that occupies
        *  the last line of text to our 'blank' character */
	memcpy(terminal0, terminal_buffer + (25 - temp) * 80, 80);
        memsetw (terminal_buffer + (25 - temp) * 80, blank, 80);
        terminal_row = 25 - 3;
    }
}

void terminal_putchar(char c) {
	unsigned char uc = c;
	switch (uc)
	{
	case '\n':
		terminal_column = 0;
		terminal_row++;
		scroll();
		move_cursor((terminal_row * 80) + terminal_column);
		break;
	case '\b':
		if(terminal_column > 0) {
            	terminal_column--;
            	terminal_putchar(' ');
            	terminal_column--;
        	}
		else {
			terminal_row--;
			terminal_column = 78;
			terminal_putchar(' ');
			terminal_column--;
			
		}
		break;
	case '\t':
		printf("    ");
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
		unsigned short pos = (terminal_row * 80) + terminal_column;
		move_cursor(pos + 1);
	}
	scroll();
}

void terminal_write(const char* data, size_t size) {
	for (size_t i = 0; i < size; i++)
		terminal_putchar(data[i]);
}

void terminal_writestring(const char* data) {
	terminal_write(data, strlen(data));
}
