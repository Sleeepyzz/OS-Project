#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <kernel/pmm.h>

#define BIT_INDEX(x) (x / 0x20)
#define BIT_OFFSET(x) (x % 0x20)

uint32_t nframes;
uint32_t *frames;
uint32_t free_frames;
uint32_t phys_mem_mb;

void use_bit(size_t *bitmap, size_t bit)
{
    bitmap[BIT_INDEX(bit)] |= (0x1 << BIT_OFFSET(bit));
}

void free_bit(size_t *bitmap, size_t bit)
{
    bitmap[BIT_INDEX(bit)] &= ~(0x1 << BIT_OFFSET(bit)); 
}

size_t test_bit(size_t *bitmap, size_t bit)
{
    return !(bitmap[BIT_INDEX(bit)] & (0x1 << BIT_OFFSET(bit)));
}

void init_pmm(uint32_t mem_size) {
	nframes = (mem_size * 1024) / 0x1000;
	free_frames = nframes;
	frames = (uint32_t*)malloc(sizeof(uint32_t) * nframes / 8 + sizeof(uint32_t) * nframes % 8, 0, 0);
	phys_mem_mb = mem_size / 1024;
	free_frames = nframes;
	for (uint32_t i = 0; i < nframes; i++) {
        	free_bit(frames, i);
	}
}

uint32_t first_frame() {
	for (uint32_t i = 0; i < nframes; i++) {
		if (test_bit(frames, i)) {
			return i;
		}
	}
	return -1;
}

uint32_t alloc_frame() {
	uint32_t idx = first_frame();
	if (idx != -1) {
		use_bit(frames, idx);
		free_frames--;
		return idx * 0x1000;
	}
	return -1;
}

void free_frame(uint32_t addr) {
	uint32_t idx = addr / 0x1000;
	free_bit(frames, idx);
	free_frames++;
}
