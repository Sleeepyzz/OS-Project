#ifndef _KERNEL_TTY_H
#define _KERNEL_TTY_H

#include <stddef.h>
#include <stdint.h>

void terminal_initialize(void);
void terminal_putchar(char c);
void terminal_putchar2(char c);
void terminal_write(const char* data, size_t size);
void terminal_writestring(const char* data);
void scroll(void);
void terminal_clear();
void clearRow(int number);
void set_buffer(uint16_t* buffer);

extern uint8_t terminal_color;

/*Virtual terminal structure*/
typedef struct virt_term {
	uint16_t* buffer;
	uint8_t color;
	int x;
	int y;
} virtual_terminal_t;

#endif
