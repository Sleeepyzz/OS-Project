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

#define CHECK_FLAG(flags,bit)   ((flags) & (1 << (bit)))

extern uint32_t nframes;
extern uint32_t phys_mem_mb;
extern uint32_t kernel_end;
extern size_t terminal_column;
extern size_t terminal_row;
uint32_t phys_mem_kb;
uint32_t total_mem;
multiboot_info_t *mbi;

void kernel_main(unsigned long addr, unsigned long magic) {
	if (magic != MULTIBOOT_BOOTLOADER_MAGIC) {
        	printf ("Error: multiboot validation failure");
        	return;
    	}
	mbi = (multiboot_info_t *) addr;
	if (CHECK_FLAG (mbi->flags, 6)) {
           	multiboot_memory_map_t *mmap;
           	for (mmap = (multiboot_memory_map_t *) mbi->mmap_addr;
                (unsigned long) mmap < mbi->mmap_addr + mbi->mmap_length;
                mmap = (multiboot_memory_map_t *) ((unsigned long) mmap
                + mmap->size + sizeof (mmap->size))) {

			total_mem += mmap->len;
         	}
		
     	}
	gdt_install();
	init_pmm(total_mem);
	init_vmm();
	terminal_initialize();
	printf("Starting initialization of PLP OS \n");
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
