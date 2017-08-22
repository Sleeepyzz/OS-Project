#include <string.h>
#include <stdint.h>

char* int_to_string(int num)
{
    char* str = malloc(100, 0, 0);
    int i, rem, len = 0, n;
    n = num;
    while (n != 0)

    {
        len++;

        n /= 10;

    }

    for (i = 0; i < len; i++)

    {

        rem = num % 10;

        num = num / 10;

        str[len - (i + 1)] = rem + '0';

    }

    str[len] = '\0';
    return str;

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
