#include <string.h>

void * malloc(int nbytes) {
	char variable[nbytes];
	return &variable;
}
