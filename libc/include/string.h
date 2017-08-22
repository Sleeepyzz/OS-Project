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
int str_to_int(char* ch);
char *int_to_string(int num);
char *strstr(char* string, char* substring);
size_t strcspn( const char *str1, const char *str2 );
char *strtok(char * str, const char * delim);
size_t strspn(const char *s1, const char *s2);
char *strchr(const char *s, int c);
char *strcpy(char *dest, const char *src);
int strcmp(const char * s1, const char * s2);
int isNumber(const char *str);

#ifdef __cplusplus
}
#endif

#endif
