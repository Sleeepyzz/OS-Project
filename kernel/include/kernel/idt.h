#ifndef ARCH_I386_IDT_H
#define ARCH_I386_IDT_H

#include <stdint.h>

/*Function prototypes for the isrs, irqs and idt*/
extern void irq_uninstall_handler(int irq);
extern void irq_remap();
extern void print_irqs();

extern void isr0();
extern void isr1();
extern void isr2();
extern void isr3();
extern void isr4();
extern void isr5();
extern void isr6();
extern void isr7();
extern void isr8();
extern void isr9();
extern void isr10();
extern void isr11();
extern void isr12();
extern void isr13();
extern void isr14();
extern void isr15();
extern void isr16();
extern void isr17();
extern void isr18();
extern void isr19();
extern void isr20();
extern void isr21();
extern void isr22();
extern void isr23();
extern void isr24();
extern void isr25();
extern void isr26();
extern void isr27();
extern void isr28();
extern void isr29();
extern void isr30();
extern void isr31();

extern void irq0();
extern void irq1();
extern void irq2();
extern void irq3();
extern void irq4();
extern void irq5();
extern void irq6();
extern void irq7();
extern void irq8();
extern void irq9();
extern void irq10();
extern void irq11();
extern void irq12();
extern void irq13();
extern void irq14();
extern void irq15();

/*Registers struct
What the stack will look like at a interrupt*/
struct regs
{
	unsigned int gs, fs, es, ds;
	unsigned int edi, esi, ebp, ebx, edx, ecx, eax;
	unsigned int int_no, err_code;
	unsigned int eip, cs, eflags, useresp, ss; 
};

typedef void (*isr_t)(struct regs *);
extern void irq_install_handler(uint8_t irq, isr_t handler);

/*IDT entry struct*/
struct idt_entry
{
    unsigned short base_lo; /*Lower 16 bits of base*/
    unsigned short sel;     /*The selector*/
    unsigned char  always0; /*The value that is always 0*/
    unsigned char  flags;   /*Flags*/
    unsigned short base_hi; /*Higher bits of the base*/
} __attribute__((packed));

/*IDT ptr struct
A struct like this is passed to load_idt()
Which uses the lidt asm function to load the table*/
struct idt_ptr
{
    unsigned short limit;  /*Limit of the idt*/
    unsigned long  base;   /*The base addr of the idt*/
} __attribute__((packed));
#endif
