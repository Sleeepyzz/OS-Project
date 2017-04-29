#ifdef ARCH_I386_TIMER_H
#define ARCH_I386_TIMER_H

int min;
int hr;
int secs;

extern void timer_install();
extern void timer_wait();
extern void print_time(hr, min, secs);

#endif
