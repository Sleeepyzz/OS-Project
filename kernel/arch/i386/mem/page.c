/*
page.c Setup paging

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
#include <string.h>
#include <stdlib.h>
#include <kernel/mm.h>
#include <kernel/idt.h>
#include <assert.h>

page_directory_t kernel_directory;
page_directory_t *current_directory;
uint32_t mapped_pages;
extern uint32_t kernel_end;

void set_current_directory(page_directory_t *dir)
{
    	current_directory = dir;
}

void switch_page_directory(page_directory_t *dir)
{
    	set_current_directory(dir);
    	asm volatile ("mov %0, %%cr3":: "r"(current_directory->phys_addr));
}

void invlpg(uint32_t addr) {
	asm volatile ("invlpg (%0)" : : "b" (addr) : "memory");
}

void map_kernel_into_dir(page_directory_t *new_pd) {
	uint32_t *page_dir = new_pd->virt_addr;
	uint32_t *kernel_dir = kernel_directory.virt_addr;
	page_dir[768] = kernel_dir[768];
}

pg_info_t get_page_index(uint32_t addr) {
	addr &= ~(0xFFF);
	pg_info_t page;
	page.page_dir_idx = addr >> 22;
	page.page_table_idx = (addr >> 12) &0x3FF;
	return page;
}

void map_page(uint32_t virt_addr, uint32_t phys_addr, int attribs) {
	virt_addr &= ~(0xFFF);
	phys_addr &= ~(0xFFF);

	pg_info_t pginf = get_page_index(virt_addr);
	uint32_t *page_dir = current_directory->virt_addr;
	
	if (!(page_dir[pginf.page_dir_idx] & 1)) {
		uint32_t table_phys_addr = alloc_frame();

		if (table_phys_addr == phys_addr) {
			uint32_t tmp = table_phys_addr;
			table_phys_addr = alloc_frame();
			free_frame(tmp);
		}

		
		uint32_t *kernel_page_table = current_directory->tables[768].virt_addr;
		
		uint32_t PT10_tmp = kernel_page_table[10];
		
		kernel_page_table[10] = table_phys_addr | 3;
		
		uint32_t *page_table = (uint32_t *) 0xC000A000;
		
		memset((uint8_t *) page_table, 0, 4096);
		
		for (uint32_t i = 0; i < 1024; i++)
		{
			page_table[i] = 2;
		}
		
		page_dir[pginf.page_dir_idx] = table_phys_addr | 3;
		
		uint32_t page_table_recursive_addr = 0xFFC00000 + (0x1000 * pginf.page_dir_idx);

		current_directory->tables[pginf.page_dir_idx].virt_addr = (uint32_t *) page_table_recursive_addr;
		current_directory->tables[pginf.page_dir_idx].phys_addr = table_phys_addr;
		
		kernel_page_table[10] = PT10_tmp;
	}
	
	uint32_t *page_table = current_directory->tables[pginf.page_dir_idx].virt_addr;
	page_table[pginf.page_table_idx] = phys_addr | attribs;
	//Flush
	invlpg(virt_addr);
}

void map_area(uint32_t start_addr, uint32_t end_addr, uint8_t flags) {
	start_addr &= ~(0xFFF);
	end_addr &= ~(0xFFF);
	for (uint32_t i = start_addr; i < end_addr; i += 0x1000) {
		uint32_t phys_addr = alloc_frame();
		map_page(i, phys_addr, flags);
	}
}

void unmap_page(uint32_t virt_addr) {
	virt_addr &= ~(0xFFF);
	pg_info_t pginf = get_page_index(virt_addr);
	
	uint32_t *page_dir = current_directory->virt_addr;

	assert(page_dir[pginf.page_dir_idx] & 1);

	uint32_t *page_table = current_directory->tables[pginf.page_dir_idx].virt_addr;

	page_table[pginf.page_table_idx] = 2; // r/w, not present;

	invlpg(virt_addr);
}

void unmap_area(uint32_t start_addr, uint32_t end_addr) {
	start_addr &= ~(0xFFF);
	end_addr &= ~(0xFFF);
	for (uint32_t i = start_addr; i < end_addr; i += 0x1000) {
		unmap_page(i);
	}
}

void init_paging()
{
	assert((uint32_t)frames != 0x0);
	

	uint32_t page_dir_virt_addr = (uint32_t) frames->addr + frames->bytes;

	// make sure that address is page aligned.
	if (page_dir_virt_addr % 0x1000 != 0)
	{
		page_dir_virt_addr = (page_dir_virt_addr & ~(0xFFF)) + 0x1000;
	}

	uint32_t page_table_virt_addr = page_dir_virt_addr + 0x1000;
	

	if ((page_dir_virt_addr > 0xC03FE000) || (page_table_virt_addr >0xC03FE000))
	{
		printf("\nUnable to initialize paging. Page dir addr or Page table addr too high. \n\n Page Dir addr: %x\n Page Table addr: %x\n", page_dir_virt_addr, page_table_virt_addr);
	}

	//Don't do "- 0xC0000000" anywhere else it won't work
	uint32_t page_dir_phys_addr = page_dir_virt_addr - 0xC0000000;
	uint32_t page_table_phys_addr = page_table_virt_addr - 0xC0000000;
	
	//Page dir pointer
	uint32_t *page_dir_ptr = (uint32_t *) page_dir_virt_addr;
	
	//Clear 4kib of space
	memset((uint8_t *) page_dir_ptr, 0, 0x1000);
	
	//Create empty page dir
	for (uint32_t i = 0; i < 1024; i++)
	{
		page_dir_ptr[i] = 0 | 2;
	}
	
	//page table pointer
	uint32_t *page_table_ptr = (uint32_t *) page_table_virt_addr;
	
	//Clear 4kib of space
	memset((uint8_t *) page_table_ptr, 0, 0x1000);
	
	for (uint32_t i = 0; i < 1024; i++)
	{
		page_table_ptr[i] = (i * 0x1000) | 3;
	}
	
	// Put phys addr of pt on page dir
	page_dir_ptr[768] = page_table_phys_addr | 3;
	
	// Set up recursive mappings
	page_dir_ptr[1023] = page_dir_phys_addr | 3;
	
	// Save the virt and phys addr of dir
	kernel_directory.virt_addr = (uint32_t *) page_dir_virt_addr;
	kernel_directory.phys_addr = page_dir_phys_addr;
	
	// Empty page tables
	for (uint32_t i = 0; i < 1024; i++)
	{
		kernel_directory.tables[i].virt_addr = 0;
		kernel_directory.tables[i].phys_addr = 0;
	}
	
	// put the stuff for the kernel page table on the data structures
	kernel_directory.tables[768].virt_addr = (uint32_t *) 0xFFF00000;
	kernel_directory.tables[768].phys_addr = page_table_phys_addr;
	
	kernel_directory.tables[1023].virt_addr = (uint32_t *) 0xFFC00000;
	kernel_directory.tables[1023].phys_addr = page_dir_phys_addr;
	
	// set the kernel page directory as the current page directory
	current_directory = (page_directory_t *) &kernel_directory;
	
	// i need to figure out what to put on CR4 to switch the system to 4 KB pages
	uint32_t new_cr4_val = readcr4() & ~(0x00000010);
	
	asm volatile (
		"mov %0, %%cr3\n\t"
		"mov %1, %%cr4"
		: /* no outputs */
		: "r" (page_dir_phys_addr), "r" (new_cr4_val)
	);
	map_area(0xC0400000, 0xC1400000, 3);

}

void page_fault(struct regs r)
{
	uint32_t faulting_address;
	asm volatile ("mov %%cr2, %0" : "=r" (faulting_address));

	int present = !(r.err_code & 0x1);		// page not present
	int rw = r.err_code & 0x2;			// write operation?
	int user = r.err_code & 0x4;			// user mode?
	int reserved = r.err_code & 0x8;
	int id = r.err_code & 0x10;			// from instr fetch?

	printf("Page fault! (");
	if (present) printf("present ");
	if (rw) printf("read-only ");
	if (user) printf("user-mode ");
	if (reserved) printf("reserved ");
	printf(") at 0x %d\n Page fault", faulting_address);
}
