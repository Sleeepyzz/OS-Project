#include <stdio.h>
#include <stdint.h>
#include <kernel/panic.h>

void panic(const char* message, const char* file, uint32_t line)
{
	asm volatile ("cli");

	printf("PANIC(");
	printf(message);
	printf(") at ");
	printf(file);
	printf(":");
	printf(int_to_string(line));
	printf("\n");

	asm ("hlt");
}
