#include <limits.h>
#include <stdbool.h>
#include <stdarg.h>
#include <stddef.h>
#include <string.h>
#include <kernel/vga.h>
#define EOF (-1)

unsigned int atoi (char *buf) {
	unsigned int result = 0;
	for (unsigned int i = 0; buf[i] != '\0'; ++i) {
        	result = result*10 + buf[i] - '0';
	}
	return result;
}

void itoa (char *buf, int base, int d) {
	char *p = buf;
	char *p1, *p2;
       	unsigned long ud = d;
       	int divisor = 10;
       	/* If %d is specified and D is minus, put `-' in the head. */
       	if (base == 'd' && d < 0) {
		*p++ = '-';
        	buf++;
           	ud = -d;
       	}
       	else if (base == 'x' || base == 'X')
       	 	divisor = 16;
     
       	/* Divide UD by DIVISOR until UD == 0. */
       	do {
        	int remainder = ud % divisor;
     		if (base == 'X'){
			*p++ = (remainder < 10) ? remainder + '0' : remainder + 'A' - 10;
		}
		else {
        		*p++ = (remainder < 10) ? remainder + '0' : remainder + 'a' - 10;
		}
        }
       	while (ud /= divisor);
     
       	/* Terminate BUF. */
       	*p = 0;
     
       	/* Reverse BUF. */
       	p1 = buf;
       	p2 = p - 1;
       	while (p1 < p2) {
           	char tmp = *p1;
           	*p1 = *p2;
           	*p2 = tmp;
           	p1++;
           	p2--;
	}
}
 
static bool print(const char* data, size_t length) {
	const unsigned char* bytes = (const unsigned char*) data;
	for (size_t i = 0; i < length; i++)
		if (putchar(bytes[i]) == EOF)
			return false;
	return true;
}

int printf(const char* restrict format, ...) {
	va_list parameters;
	va_start(parameters, format);
 
	int written = 0;
 
	while (*format != '\0') {
		size_t maxrem = INT_MAX - written;
 
		if (format[0] != '%' || format[1] == '%') {
			if (format[0] == '%')
				format++;
			size_t amount = 1;
			while (format[amount] && format[amount] != '%')
				amount++;
			if (maxrem < amount) {
				return -1;
			}
			if (!print(format, amount))
				return -1;
			format += amount;
			written += amount;
			continue;
		}
 
		const char* format_begun_at = format++;
 
		if (*format == 'c') {
			format++;
			char c = (char) va_arg(parameters, int);
			if (!maxrem) {
				return -1;
			}
			if (!print(&c, sizeof(c)))
				return -1;
			written++;
		} else if (*format == 's') {
			format++;
			const char* str = va_arg(parameters, const char*);
			size_t len = strlen(str);
			if (maxrem < len) {
				return -1;
			}
			if (!print(str, len))
				return -1;
			written += len;
		} else if (*format == 'd') {
			format++;
			int val = va_arg(parameters, int);
			char buf[100];
			itoa(buf, 'd', val);
			size_t len = strlen(buf);
			if (!print(buf, len)) {
				return -1;
			}
			written += len;
		} else if (*format == 'u') {
			format++;
			int val = va_arg(parameters, int);
			char buf[100];
			itoa(buf, 'u', val);
			size_t len = strlen(buf);
			if (!print(buf, len)) {
				return -1;
			}
			written += len;
		} else if (*format == 'x') {
			format++;
			int val = va_arg(parameters, int);
			char buf[100];
			itoa(buf, 'x', val);
			size_t len = strlen(buf);
			if (!print(buf, len)) {
				return -1;
			}
			written += len;
		} else if (*format == 'X') {
			format++;
			int val = va_arg(parameters, int);
			char buf[100];
			itoa(buf, 'X', val);
			size_t len = strlen(buf);
			if (!print(buf, len)) {
				return -1;
			}
			written += len;
		} else if (*format == 'f') {
			format++;
			int val = va_arg(parameters, int);
			char buf[100];
			itoa(buf, 'd', val);
			char buf2[100];
			size_t len = strlen(buf);
			if (!print(buf, len)) {
				return -1;
			}
			written += len;
		} else {
			format = format_begun_at;
			size_t len = strlen(format);
			if (maxrem < len) {
				return -1;
			}
			if (!print(format, len))
				return -1;
			written += len;
			format += len;
		}
	}
	va_end(parameters);
	return written;
}
