#ifdef ARCH_I386_IRQ_H
#define ARCH_I386_IRQ_H

extern void irq_install_handler(int irq, (*handler)(struct regs *r));
extern void irq_uninstall_handler(int irq);
extern void irq_remap();
extern void print_irqs();

#endif
