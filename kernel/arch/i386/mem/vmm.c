/*
vmm.c The kernel heap manager

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
#include <assert.h>
#include <kernel/mm.h> /*stddef included in this*/


/*We have a head and a tail to keep track of the first 
and last blocks*/
block_t *head = NULL;
block_t *tail = NULL;


void printheap() {
	block_t *current = head;

	printf("\n\nKernel Heap Info:");
	printf("\nHead: %x size: %d is_free: %d", head->data, head->size, head->is_free);
	printf("\nTail: %x size: %d is_free: %d\n", tail->data, tail->size, tail->is_free);
	printf("\nHeap:\n");
	while (current) {
		printf("\nAddr: %x size: %d is_free: %d", current->data, current->size, current->is_free);
		current = current->next;
	}
}

uint32_t curr_heap_end;

/*Doubly linked list functions 
only work for the block_t data type but can be changed*/
void insert_first(block_t* new) {
	if (head == NULL) {
		tail = new;
	}
	else {
		insert_before(new, head);
	}
	head = new;
}

void insert_last(block_t* new) {
	if (head == NULL) {
		tail = new;
	}
	else {
		insert_after(new, tail);
	}
	tail = new;
}

void insert_before(block_t* new, block_t* b) {
	if (b == head) {
		new->next = b;
		new->prev = NULL;
		b->prev = new;
		head = new;
	}
	else {
		b->prev->next = new;
		new->next = b;
		b->prev = new;
	}
}

void insert_after(block_t* new, block_t* b) {
	if (b == tail) {
		new->next = NULL;
		b->next = new;
		new->prev = b;
		tail = new;
	}
	else {
		b->next->prev = new;
		new->next = b->next;
		b->next = new;
		new->prev = b;
	}
}

/* The init vmm function called from the kernel */
void init_vmm() {
	/*The head starts at 0xC0800000 and ends at 0xC0C00000
	It can expand up to 0xC1000000
	the addr that the head starts at needs to be mapped already*/
	head = (block_t *)0xC0800000;
	curr_heap_end = 0xC0800000 + INITIAL_HEAP_SIZE;
	head->is_free = 1; // 1 = YES, 0 = NO
	head->size = INITIAL_HEAP_SIZE - BLOCK_SIZE;
	head->prev = NULL;
	head->next = NULL;
	tail = head;
}

block_t *find_block(size_t size) {
	/*Using a best fit algoritim
	We start from the head and search the blocks for one that
	is free and is at least the requested size 
	Check the size difference and see if it should be the result*/
	block_t *current = head;
	block_t *result= NULL;
	int best_difference = NULL;
	while (current != NULL) {
		if (current->size >= size && current->is_free == 1) {
			if (best_difference == NULL) {
				best_difference = current->size - size;
				result = current;
			}
			if (current->size - size < best_difference) {
				result = current;
			}
		}
		current = current->next;
	}
	/*Will return NULL if no blocks were found*/
	return result;
}

void split(block_t *b, size_t s) {
	block_t *new;
	/*b->data points to the addr at the 
	end of the block structure. We use the addr
	s bytes after that for the new block*/
	new = (block_t *)(b->data + s);
	new->size = b->size - s - BLOCK_SIZE;
	new->is_free = 1;
	new->ptr = new->data;
	b->size = s;
	insert_after(new, b);
}


void merge(block_t *block) {
	/*Check if block can be merged with next*/
	if (block->next && block->next->is_free){
		/*Increase its size by the BLOCK_SIZE
		and the next blocks size*/
		if(block->next == tail) {
			tail = block;
		}
		block->size += BLOCK_SIZE + block->next->size;
		block->next = block->next->next;
		if (block->next) {
			block->next->prev = block;
		}
	}
}

void expand(size_t size) {
	/*Add a block of "size" bytes to the end of the linked list */
	block_t *new_block;
	curr_heap_end += (new_block->size + BLOCK_SIZE);
	if (curr_heap_end >= MAX_HEAP_SIZE) {
		int pages = size / 0x1000;
		if (pages == 0) pages = 1;
		for (int i = 0; i < pages; i++) {
			uint32_t phys_addr = alloc_frame();
			assert(phys_addr);
			uint32_t virt_addr = (uint32_t)tail->data + tail->size + i * 0x1000;
			map_page(virt_addr, phys_addr, 3);
		}
	}
	/*Place the block at the end of the tail
	Set its size to the requested size
	And mark it as free*/
	new_block = (block_t *)tail->data + tail->size;
	new_block->size = 0x1000;
	new_block->is_free = 1;
	insert_last(new_block);
}

block_t *get_block(void *ptr) {
	/*Get a block from a ptr */
	char* tmp;
	tmp = ptr;
	return (ptr = tmp -= BLOCK_SIZE);
}

int is_valid_ptr(void *ptr) {
	if (head) {
		/*If the pointer is between the head and tail then it is probably valid*/
		if ((uint32_t)ptr >= (uint32_t)head && (uint32_t)ptr <= (uint32_t)tail->data) {
			/*Make sure pointer is valid*/
			return (ptr == (get_block(ptr))->ptr);
		}
	}
	return 0;
}

void copy_block(block_t *dest, block_t *src) {
	/*Use the blocks as unsigned ints
	and then copy them until the iterator
	is equal to the size of the smaller block*/
	uint32_t *ddata, *sdata;
	ddata = dest->ptr;
	sdata = src->ptr;
	for (unsigned int i = 0; i < dest->size && i < src->size; i++) {
		ddata[i] = sdata[i];
	}
}

void *kmalloc(size_t size) {
	/*kmalloc should not be used when there is no head*/
	assert(head != NULL);
	/*Align the size given*/
	size_t s = align4(size);
	/*Find a block for the aligned size*/
	block_t *block = find_block(s);
	if (block) {
		if ((block->size - s) >= (BLOCK_SIZE + 4)) {
			split(block, s);
		}
		block->is_free = 0;
	}
	else {
		printheap();
		/*No block was found expand the heap
		And try again*/
		expand(s);
		block = find_block(s);
		/*If there still is no block PANIC*/
		assert(block != NULL);
	}
	/*Return a pointer to the end of the blocks metadata*/
	return block->data;
}

void kfree(void* ptr) {
	//Check if pointer given is one issued by kkmalloc
	if (!(is_valid_ptr(ptr))) return;
	block_t *block = get_block(ptr);
	block->is_free = 1;
	merge(block);
}

void *kcalloc(int number, size_t size) {
	void *ptr = kmalloc(number * size);
	assert(ptr);
	memset(ptr, 0, (number * size));
	return ptr;
}

void *krealloc(void *ptr, size_t new_size) {
	if (!is_valid_ptr(ptr)) return 0;
	block_t *block = get_block(ptr);
	size_t s = align4(new_size);
	if (block->size == new_size){
		return ptr;
	}
	else if (new_size < block->size) {
		if (block->size - s >= BLOCK_SIZE + 4){
			split(block, new_size);
		}
	}
	else {
		if (block->next && block->next->is_free && block->size + BLOCK_SIZE + block->next->size >= s) {
			block->is_free = 1;
			merge(block);
			if (block->size - s >= BLOCK_SIZE + 4) {
				split(block, s);
			}
			block->is_free = 0;
			return block->data;
		}
		else {
			void *new_ptr = kmalloc(s);
			block_t *new = get_block(new_ptr);
			copy_block(block, new);
			kfree(ptr);
			return new_ptr;
		}
	}
	return ptr;
}

