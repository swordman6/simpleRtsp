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
