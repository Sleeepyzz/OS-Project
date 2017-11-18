#include <stdio.h>
#include <stdint.h>
#include <assert.h>

void panic_assert(const char *file, uint32_t line, const char *desc)
{
    	// An assertion failed, and we have to panic.
    	asm volatile("cli"); // Disable interrupts.
	
    	printf("ASSERTION-FAILED(%s) at %s:%d\n", desc, file, line);
    	for(;;);
}
