#include <stdio.h>
#include <kernel/tty.h>
#include <kernel/gdt.h>
#include <kernel/idt.h>
#include <kernel/timer.h>
#include <kernel/keyboard.h>
#include <kernel/shell.h>
#include <kernel/mm.h>

#define CHECK_FLAG(flags,bit)   ((flags) & (1 << (bit)))

extern uint32_t nframes;
extern uint32_t phys_mem_mb;
extern uint32_t kernel_end;
extern size_t terminal_column;
extern size_t terminal_row;
extern void isr1();
uint32_t total_mem = 2;
multiboot_info_t *mbi;

void kernel_main(unsigned long addr, unsigned long magic) {
	multiboot_info_t *mbi = (multiboot_info_t *) addr;
	terminal_initialize();
	gdt_install();
	idt_install();
	printf("  _____  _      _____     ____   _____ \n");
	printf(" |  __ \\| |    |  __ \\   / __ \\ / ____|\n");
	printf(" | |__) | |    | |__) | | |  | | (___  \n");
	printf(" |  ___/| |    |  ___/  | |  | |\\___ \\ \n");
	printf(" | |    | |____| |      | |__| |____) |\n");
	printf(" |_|    |______|_|       \\____/|_____/  By Ryken Thompson\n");
	printf("\nLoading...\n");
	init_pmm(mbi);
	init_paging();
	init_vmm();
	/*Test malloc, calloc, and realloc*/
	int* a = (int*)kmalloc(4);
	*a = 2;
	int *b = (int*)kmalloc(8);
	kfree(b);
	/*printmalloc();*/
	char* c = calloc(1, 8);
	*c = "HI";
	*c = realloc(c, 16);
	/*printmalloc();*/
	kfree(c);

	/*Install the pit and keyboard
	then enable interrupts*/
	timer_install(100);
	keyboard_init();
	asm ("sti");
	/*Clear the framebuffer*/
	terminal_clear();
	/*Then launch the shell*/
	shell();
	for (;;);
}
