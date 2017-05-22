#ifdef ARCH_I386_PMM_H
#define ARCH_I386_PMM_H

void alloc_frame();
void free_frame(uint32_t addr);
void init_pmm(uint32_t mem_size);

#endif
