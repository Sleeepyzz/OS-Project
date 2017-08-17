#ifndef _STDLIB_H
#define _STDLIB_H 1

#include <sys/cdefs.h>
#include <stdint.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

__attribute__((__noreturn__))
void abort(void);

uint32_t malloc_a(uint32_t size);  // page aligned.
uint32_t malloc_p(uint32_t size, uint32_t *phys); // returns a physical address.
uint32_t malloc_ap(uint32_t size, uint32_t *phys);
uint32_t malloc(uint32_t size, int align, uint32_t *phys);

#ifdef __cplusplus
}
#endif

#endif
