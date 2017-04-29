#include <kernel/isr.h>
#include <kernel/idt.h>
#include <stdio.h>
#include <stdint.h>
#include <stddef.h>

struct regs
{
	unsigned int gs, fs, es, ds;
	unsigned int edi, esi, ebp, ebx, edx, ecx, eax;
	unsigned int int_no, err_code;
	unsigned int eip, cs, eflags, useresp, ss; 
};



char *exceptions[] =
{
    "Division By Zero",
    "Debug",
    "Non Maskable Interrupt",
    "Breakpoint",
    "Into Detected Overflow",
    "Out of Bounds",
    "Invalid Opcode",
    "No Coprocessor",
    "Double Fault",
    "Coprocessor Segment Overrun",
    "Bad TSS",
    "Segment Not Present",
    "Stack Fault",
    "General Protection Fault",
    "Page Fault",
    "Unknown Interrupt",
    "Coprocessor Fault",
    "Alignment Check",
    "Machine Check",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved"
};

void isr_handler (struct regs* r) {
	if (r->int_no < 32) {
		puts(exceptions[r->int_no]);
		printf("ERROR. RESTART OS");
		asm ("hlt");
	}
	if (r->int_no > 31) {
		printf("irq");
		asm ("hlt");
	}
}
