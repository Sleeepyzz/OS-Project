#ifndef _ASSERT_H
#define _ASSERT_H 1

#include <stdint.h>

extern void panic_assert(const char *file, uint32_t line, const char *desc);
#define assert(b) ((b) ? (void)0 : panic_assert(__FILE__, __LINE__, #b))

#endif
