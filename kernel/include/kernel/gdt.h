#ifdef ARCH_I386_GDT_H
#define ARCH_I386_GDT_H

extern void gdt_set_gate(uint32_t i, uint64_t base, uint64_t limit, uint8_t access, uint8_t granularity)
extern void gdt_install();

#endif
