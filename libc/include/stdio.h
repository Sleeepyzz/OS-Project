#ifndef _STDIO_H
#define _STDIO_H 1

#include <sys/cdefs.h>

#define EOF (-1)
#define NULL 0

#ifdef __cplusplus
extern "C" {
#endif

int printf(const char* format, ...);
int putchar(int);
int puts(const char*);
unsigned int atoi(char* buf);
void itoa(char* buf, int base, int d);

#ifdef __cplusplus
}
#endif

#endif
