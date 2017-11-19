/*
pmm.c The physical memory manager

Copyright (C) 2017 Ryken Thompson

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program. If not, see <http://www.gnu.org/licenses/>.

*/

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <kernel/mm.h>

uint32_t nframes;

extern uint32_t BootPageDirectory;
extern uint32_t start;
extern uint32_t end;

uint32_t boot_page_dir_addr = (uint32_t) &BootPageDirectory;
uint32_t kernel_start = (uint32_t) &start;
uint32_t kernel_end = (uint32_t) &end;
uint32_t total_mem;
uint32_t useable_mem;

bitmap_t *frames = 0;
//Use a bit in a bitmap
void use_bit(bitmap_t *bitmap, size_t bit)
{
    bitmap->addr[BIT_INDEX(bit)] |= (0x1 << BIT_OFFSET(bit));
}
//Free a bit in a bitmap
void free_bit(bitmap_t *bitmap, size_t bit)
{
    bitmap->addr[BIT_INDEX(bit)] &= ~(0x1 << BIT_OFFSET(bit)); 
}
//Test a bit in a bitmap
size_t test_bit(bitmap_t *bitmap, size_t bit)
{
    return !(bitmap->addr[BIT_INDEX(bit)] & (0x1 << BIT_OFFSET(bit)));
}

void init_pmm(multiboot_info_t *mbi) {
	uint32_t mem_size_mb = mbi->mem_upper / 1024 + 2;
	uint32_t mem_size_kb = mem_size_mb * 1024;
	nframes = mem_size_kb / 4;

	uint32_t frames_addr = ((kernel_end & ~(0xFFF)) + 0x1000) - sizeof(bitmap_t);
	uint32_t bitmap_addr = ((kernel_end & ~(0xFFF)) + 0x1000);
	uint32_t bitmap_size = nframes / 8;
	frames = (bitmap_t *) frames_addr;
	frames->addr = (uint8_t *) bitmap_addr;
	frames->bytes = bitmap_size;
	
	//Set all bits
	memset((uint8_t *)frames->addr, 0xFF, frames->bytes);
	
	multiboot_memory_map_t *mmap = (multiboot_memory_map_t *) (mbi->mmap_addr + 0xC0000000);
	//Repeat while there are entries not checked
	while ((uint32_t)mmap < mbi->mmap_addr + 0xC0000000 + mbi->mmap_length) {
		total_mem += mmap->len;
		if (mmap->type == 0x1) { //Type 0x1 is available for use
			uint32_t region_start = mmap->addr;
			uint32_t mmap_len = mmap->len;
			uint32_t region_frames = mmap_len / 0x1000;
			useable_mem += mmap->len;

			for (uint32_t i = 0; i < region_frames; i++) {
				uint32_t four_k_frame_addr = region_start + i * 0x1000;
				uint32_t bit = four_k_frame_addr / 0x1000;
				free_bit(frames, bit);
			}
		}
		
		mmap = (multiboot_memory_map_t *) ((uint32_t)mmap + mmap->size +sizeof(uint32_t));
		
	}

	uint32_t *boot_page_dir = (uint32_t *) boot_page_dir_addr;
	//Repeat for every entry in the page directory
	for (int i = 0; i < 1024; i++) {
		//Is there a 4MB page
		if (boot_page_dir[i] & 0x1) {
			//Whats its phys addr
			uint32_t phys_addr = boot_page_dir[i] & ~(0xFFF);
			
			//Repeat for every 4KB page
			for (int j = 0; j < 1024; j++) {
				//What is the 4KB page phys addr
				uint32_t four_k_frame_addr = j * 0x1000 + phys_addr;

				//What bit represents it
				uint32_t bit = four_k_frame_addr / 0x1000;

				//Set that bit
				use_bit(frames, bit);
			}
		}
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

//Returns a address of a frame
uint32_t alloc_frame() {
	uint32_t idx = first_frame();
	assert(idx != (uint32_t)-1);
	use_bit(frames, idx);
	return idx * 0x1000;
}

//Free a frame
void free_frame(uint32_t addr) {
	uint32_t aligned_addr = addr & ~(0xFFF);
	uint32_t idx =  aligned_addr / 0x1000;
	free_bit(frames, idx);
}
