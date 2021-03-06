/**************************************************************************************************
 *
 * File: asm.h
 *
 * Copyright (c) 2003, 2004	Erik Dubbelboer
 *
 * Contents:	x86 inline assembler
 *
 *************************************************************************************************/

#ifndef _ASM_H_
#define _ASM_H_




//
// volatile will prevent gcc from optimizing the asm code.
//
#define asm(x...) __asm volatile (x)




/**************************************************************************************************
 * halt
 *************************************************************************************************/

static inline void halt(void)
{
	asm("hlt"::);
}




/**************************************************************************************************
 * cr0
 *************************************************************************************************/

static inline uint get_cr0(void)
{
	uint ret;
	asm("mov %%cr0, %0" : "=r" (ret));

	return ret;
}


static inline void set_cr0(uint cr0)
{
	asm("mov %0, %%cr0" :: "r" (cr0));
}




/**************************************************************************************************
 * cr3
 *************************************************************************************************/

static inline uint get_cr3(void)
{
	uint ret;
	asm("mov %%cr3, %0" : "=r" (ret));

	return ret;
}


static inline void set_cr3(uint cr3)
{
	asm("mov %0, %%cr3" :: "r" (cr3));
}




#endif // _ASM_H_
