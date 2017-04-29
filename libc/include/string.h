#ifndef _STRING_H
#define _STRING_H 1

#include <sys/cdefs.h>

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

int memcmp(const void*, const void*, size_t);
void* memcpy(void* __restrict, const void* __restrict, size_t);
void* memmove(void*, const void*, size_t);
void* memset(void*, int, size_t);
unsigned short *memsetw(unsigned short *dest, unsigned short val, int count);
size_t strlen(const char*);
void * malloc(int nbytes);
uint8_t strEql(char* ch1, char* ch2);
char* int_to_string(int n);
int str_to_int(char* ch);

#ifdef __cplusplus
}
#endif

#endif
