#include <stdio.h>

#include "public.h"

int pickstr(char *dst, char *src, char from, char to, int n)
{
    int i = 0;
    
    while(*src++ != from)
    {
        i++;
    }

    while((i < n) && (*src != to))
    {
        i++;
        *dst++ = *src++; 
    }

    *dst = '\0';
    return i;
}

int transfrom_16_int(char * s)
{
	int i, n;
	int temp = 0;

	for (i = 0; s[i]; i++)
	{
		if (s[i] >= 'A' && s[i] <= 'F')
			n = s[i] - 'A' + 10;
		else if (s[i] >= 'a' && s[i] <= 'f')
			n = s[i] - 'a' + 10;
		else n = s[i] - '0';

		temp = temp * 16 + n;
	}
	return temp;
}

