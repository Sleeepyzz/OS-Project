#ifndef ARCH_I386_MM_H
#define ARCH_I386_MM_H

#include <stdint.h>
#include <stddef.h>
#include <kernel/idt.h>
#include <kernel/multiboot.h>

/*PMM:
Stuctures*/
typedef struct {
	uint8_t* addr;  //The bitmap entries
	uint32_t bytes; //Bitmaps size
} bitmap_t;

/*Define some things*/
#define BIT_INDEX(x) (x / 8)
#define BIT_OFFSET(x) (x % 8)

/*Prototypes*/
extern uint32_t first_frame();
uint32_t alloc_frame();
void init_pmm(multiboot_info_t *mbi);
extern bitmap_t *frames;

/*Paging:
Stuctures */
typedef struct page_info {
	uint32_t page_dir_idx;
	uint32_t page_table_idx;
} pg_info_t;

typedef struct page_table {
    	uint32_t *virt_addr;
	uint32_t phys_addr;
} page_table_t;

typedef struct page_directory {
	uint32_t *virt_addr;
    	uint32_t phys_addr;
	page_table_t tables[1024];
} page_directory_t;

/*Prototypes*/
void init_paging();
void page_fault(struct regs r);
extern page_directory_t kernel_directory;
extern page_directory_t *current_directory;
void switch_page_directory(page_directory_t *dir);
void map_page(uint32_t virt_addr, uint32_t phys_addr, int attribs);
void map_area(uint32_t start_addr, uint32_t end_addr, uint8_t flags);
void unmap_page(uint32_t virt_addr);
void unmap_area(uint32_t start_addr, uint32_t end_addr);

/*VMM:
Stuctures*/

/*Each blocks metadata to store things like
size, next and prev pointers, and if the block is free */
typedef struct block {
	size_t size;
	struct block *next;
	struct block *prev;
	int is_free;
	void *ptr;
	char data[1];
} block_t;


/*Define some stuff*/
//The sizeof for the block_t type will be wrong so we define it ourselves
#define BLOCK_SIZE 20

//The initial heap size make sure the enough memory has been mapped by now
#define INITIAL_HEAP_SIZE 0x400000 // 4 Mib
#define MAX_HEAP_SIZE 0x1000000 // 16 Mib

#define align4(x) (((((x)-1)>>2)<<2)+4)

/*Prototypes*/
void *kmalloc(size_t size);
void kfree(void* ptr);
void *calloc(int number, size_t size);
void *realloc(void *ptr, size_t new_size);
void init_vmm();
void printheap();


#endif
