#include <stdio.h>
#include <kernel/tty.h>
#include <kernel/gdt.h>
#include <kernel/idt.h>
#include <kernel/isr.h>
#include <kernel/timer.h>
#include <kernel/keyboard.h>
#include <kernel/shell.h>
#include <kernel/pmm.h>
#include <kernel/vmm.h>
#include <kernel/multiboot.h>
#include <stdint.h>

multiboot_info_t *kernel_mboot;
extern uint32_t nframes;
extern uint32_t phys_mem_mb;
extern uint32_t kernel_end;
extern size_t terminal_column;
extern size_t terminal_row;

void kernel_main(multiboot_info_t *mbd) {
	gdt_install();
	init_pmm(mbd->mem_lower + mbd->mem_upper);
	init_vmm();
	terminal_initialize();
	printf("Starting initialization of _____ \n");
	printf("Remapping pic\n");
	irq_remap();
	printf("Installing interrupt handlers\n");
	idt_install();
	asm ("sti");
	printf("Installing timer\n");
	timer_install(100);
	printf("Interrupts enabled\n");
	printf("Launching shell...\n");
	shell();
}
