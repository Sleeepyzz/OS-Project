#ifndef ARCH_I386_PIT_H
#define ARCH_I386_PIT_H

void timer_wait(int ticks);
void timer_install(uint32_t frequency);

#endif
