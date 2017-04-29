#include <stdio.h>
#include <kernel/tty.h>
#include <kernel/gdt.h>
#include <kernel/idt.h>
#include <kernel/isr.h>
#include <kernel/timer.h>
#include <kernel/keyboard.h>
#include <kernel/shell.h>
#include <stdint.h>

void kernel_main(void) {
	terminal_initialize();
	printf("Starting initialization of _____ \n");
	printf("Terminal Initialized\n");
	gdt_install();
	printf("Global Descriptor Table Initialized\n");
	irq_remap();
	idt_install();
	timer_install(100);
	asm ("sti");
	printf("Interrupts enabled\n");
	printf("Programable Interval Timer installed\n");
	printf("Launching shell...\n");
	shell(0);
}
