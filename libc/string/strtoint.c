#include <string.h>
#include <stdint.h>

//Only used in int to string and string to int
//Not for use outside of strtoint.c
void int_to_ascii(int n, char str[]) {          
    int i, sign;
    if ((sign = n) < 0) n = -n;
    i = 0;
    do {
        str[i++] = n % 10 + '0';         
    } while ((n /= 10) > 0);

    if (sign < 0) str[i++] = '-';
    str[i] = '\0';
}

char* int_to_string(int n) {
	char* ch = malloc(50, 0, 0);
	int_to_ascii(n, ch);
	int len = strlen(ch);
	int i = 0;
	int j = len - 1;
	while (i < (len/2 + len%2)) {
		char tmp = ch[i];
		ch[i] = ch[j];
		ch[j] = tmp;
		i++;
		j++;
	}
	return ch;
}

int str_to_int(char* ch) {
	int n = 0;
	int p = 1;
	int strlength = strlen(ch);
	int i;
	for (i = strlength - 1; i >= 0; i--) {
		n += ((int)(ch[i] - '0')) * p;
		p *= 10;
	}
	return n;
}
