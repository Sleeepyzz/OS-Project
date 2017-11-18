#ifndef ARCH_I386_TIMER_H
#define ARCH_I386_TIMER_H

#include <kernel/idt.h>

#define PIT_NATURAL_FREQ 1193180
#define PIT_DATA0 0x40
#define PIT_DATA1 0x41
#define PIT_DATA2 0x42
#define PIT_COMMAND 0x43

extern void timer_install(uint32_t frequency);
extern void timer_wait(int ticks);
extern void timer_handler(struct regs *r);

#endif
