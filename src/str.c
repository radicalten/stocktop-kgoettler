#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "str.h"

char* double_to_ss(double x)
{
    double scale;
    char label;
    /* Determine power */
    int xpow = (int) floor(log10(x));
    if (xpow < 3)
    {
        label = ' ';
        scale = 1;
    }
    else if (xpow < 6)
    {
        label = 'K';
        scale = 1E3;
    }
    else if (xpow < 9)
    {
        label = 'M';
        scale = 1E6;
    }
    else if (xpow < 12)
    {
        label = 'B';
        scale = 1E9;
    }
    else if (xpow < 15)
    {
        label = 'T';
        scale = 1E12;
    }
    else
    {
        label = '!';
        scale = 1;
    }

    /* Create output string */
    x = x/scale;
    char *out = malloc(sizeof(char) * 6);
    sprintf(out, "%.4g%c", x, label);
    return out;
}
