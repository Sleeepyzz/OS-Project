#include <string.h>
#include <stdint.h>
#include <kernel/gdt.h>

/* This will be a function in start.asm. We use this to properly
*  reload the new segment registers */
extern void flush();
 
struct gdt_entry  gdt[3];
struct gdt_pointer     gdt_p;


void gdt_set_gate(uint32_t i, uint32_t base, uint32_t limit, uint32_t access, uint32_t granularity)
{
    /* Setup the descriptor base address */
    gdt[i].base_low     = base;
    gdt[i].base_middle  = (base >> 16);
    gdt[i].base_high    = (base >> 24);
 
    /* Setup the descriptor limits */
    gdt[i].limit        = limit;
 
    /*Set up the granularity and access flags */
    gdt[i].access       = access;
    gdt[i].granularity  = granularity;
}
 
void gdt_install()
{
    gdt_p.limit = (sizeof(struct gdt_entry) * 3) - 1;
    gdt_p.base  = (uint32_t) &gdt;

    /* set null descriptor */
   
    gdt_set_gate(0, 0, 0, 0, 0);
   
    /* set code descriptor */
   
    gdt_set_gate(1, 0, 0xFFFFFFFF, 0x9A, 0xCF);
   
    /* set data descriptor */
   
    gdt_set_gate(2, 0, 0xFFFFFFFF, 0x92, 0xCF);

	

    /* install the GDT */
   
    flush();
}
