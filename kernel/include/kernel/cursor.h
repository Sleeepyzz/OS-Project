#ifndef ARCH_I386_CURSOR_H
#define ARCH_I386_CURSOR_H

#include <kernel/port.h>

/* The I/O ports */
#define FB_COMMAND_PORT         0x3D4
#define FB_DATA_PORT            0x3D5

/* The I/O port commands */
#define FB_HIGH_BYTE_COMMAND    14
#define FB_LOW_BYTE_COMMAND     15

/* Move Cursor command 
Pos is a index in the framebuffer*/
void move_cursor(unsigned short pos);

#endif
