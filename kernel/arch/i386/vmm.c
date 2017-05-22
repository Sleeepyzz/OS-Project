#include <stdio.h>
#include <string.h>
#include <kernel/vmm.h>
#include <kernel/multiboot.h>

struct regs
{
	unsigned int gs, fs, es, ds;
	unsigned int edi, esi, ebp, ebx, edx, ecx, eax;
	unsigned int int_no, err_code;
	unsigned int eip, cs, eflags, useresp, ss; 
};

#define PAGE_PRESENT 0x1
#define PAGE_READ_WRITE 0x2
#define PAGE_USER 0x4
#define PAGE_WRITE_THROUGH 0x8
#define PAGE_CACHE_DISABLE 0x16
#define PAGE_ACCESSED 0x32
#define PAGE_DIRTY 0x64
#define PAGE_GLOBAL 0x128

typedef struct page_table {
    size_t pages[1024];
} page_table_t;

typedef struct page_directory {
    size_t tablesPhysical[1024];
    page_table_t *tables[1024];
} page_directory_t;


page_directory_t *kernel_directory;
page_directory_t *current_directory;
multiboot_info_t *kernel_mboot;
uint32_t mapped_pages;
extern uint32_t kernel_end;


void set_current_directory(page_directory_t *dir)
{
    current_directory = dir;
}

void switch_page_directory(page_directory_t *dir)
{
    set_current_directory(dir);
    asm volatile ("mov %0, %%cr3":: "r"(current_directory));
}

void enable_paging()
{
   uint32_t cr0;
   asm volatile("mov %%cr0, %0": "=r"(cr0));
   cr0 |= 0x80000000; // Enable paging!
   asm volatile("mov %0, %%cr0":: "r"(cr0));
}

void map(uint32_t va, uint32_t pa, uint32_t flags)
{
    uint32_t page_num = (va / 4096) % 1024;
    uint32_t table_num = (va / 4096) / 1024;
    if (!current_directory->tables[table_num]) {
        uint32_t phys;
        current_directory->tables[table_num] = (page_table_t*)malloc(sizeof(page_table_t),
                                                    1,
                                                    &phys);

        current_directory->tablesPhysical[table_num] = phys | flags;
    }
    current_directory->tables[table_num]->pages[page_num] = pa | flags;
    mapped_pages++;
}

void map_area(uint32_t from_va, uint32_t to_va, uint32_t flags)
{
    for (uint32_t va = from_va; va < to_va; va += 0x1000) {
        uint32_t pa = alloc_frame();
        map(va, pa, flags);
    }
}

void unmap(uint32_t va)
{
    uint32_t page_num = (va / 4096) % 1024;
    uint32_t table_num = (va / 4096) / 1024;

    current_directory->tables[table_num]->pages[page_num] = 0x0;

    asm volatile ("invlpg (%0)" : : "a" (va));
    mapped_pages--;
}

void init_vmm()
{
    mapped_pages = 0;

    kernel_directory = (page_directory_t*)malloc(sizeof(page_directory_t), 1, 0);
    memset(kernel_directory, 0, sizeof(page_directory_t));

    for (int i = 0; i < 1024; i++) {
        kernel_directory->tablesPhysical[i] = 0x0 | PAGE_READ_WRITE | PAGE_USER;
    }

    set_current_directory(kernel_directory);

    // Map kernel + 16 MiB
    uint32_t mem_to_map = (size_t) &kernel_end + 0x1000000;
    map_area(0x0, mem_to_map, PAGE_READ_WRITE | PAGE_PRESENT | PAGE_USER);
    switch_page_directory(kernel_directory);
    enable_paging();
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
	if (present) { printf("present "); }
	if (rw) { printf("read-only "); }
	if (user) { printf("user-mode "); }
	if (reserved) { printf("reserved "); }
	printf(") at 0x");
	printf(faulting_address);
	printf("\n");
	printf("Page fault");
}
