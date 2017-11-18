#ifndef ARCH_I386_IO_H
#define ARCH_I386_IO_H

#include <stdint.h>

void outb(uint16_t port, uint8_t data);
void outw(uint16_t port, uint16_t val);
uint8_t inb(uint16_t port);

#endif
