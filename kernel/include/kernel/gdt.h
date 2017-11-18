#ifndef ARCH_I386_GDT_H
#define ARCH_I386_GDT_H

/*Structures*/
struct gdt_entry {
    uint16_t limit;         /* bits 0-15 of segment limit */
    uint16_t base_low;          /* bits 0-23 */
    uint8_t base_middle;        /* of base address */
    uint8_t access;             /* access byte */
    uint8_t granularity;            /* granularity */
    uint8_t base_high;          /* bits 24-32 of base address */
} __attribute__ ((packed));
 
struct gdt_pointer {
    uint16_t limit;             /* size of GDT */
    uint32_t base;              /* base address of GDT */
} __attribute__ ((packed));

extern void gdt_set_gate(uint32_t i, uint32_t base, uint32_t limit, uint32_t access, uint32_t granularity);
extern void gdt_install();

#endif
