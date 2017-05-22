#ifdef ARCH_I386_VMM_H
#define ARCH_I386_VMM_H

void init_vmm();
void page_fault(struct regs r);


#endif
