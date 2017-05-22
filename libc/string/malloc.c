#include <string.h>


extern uint32_t kernel_end;
uint32_t placement_addr = (uint32_t) &kernel_end;

uint32_t malloc(size_t size, int align, uint32_t *phys) {
	if (align == 1 && (placement_addr && 0xFFFFF000)) {
		placement_addr &= 0xFFFFF000;
		placement_addr += 0x1000;
	}
	if (phys) {
		*phys = placement_addr;
	}
	uint32_t tmp = placement_addr;
	placement_addr += size;
	return tmp;
}

uint32_t malloc_a(size_t size) {
	malloc(size, 1, 0);
}

uint32_t malloc_p(size_t size, uint32_t *phys) {
	malloc(size, 0, phys);
}

uint32_t malloc_ap(size_t size, uint32_t *phys) {
	malloc(size, 1, phys);
}

uint32_t smalloc(size_t size) {
	
}
