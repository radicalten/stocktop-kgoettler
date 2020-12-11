#include <stdio.h>
#include <stdlib.h>
#include "str.h"

int main (void)
{
    double vals[] = {
        1,
        100,
        12e3,
        15.9E9,
        154.5234E12
    };
    for (int i = 0; i < 5; i++)
    {
        char *sval = double_to_ss(vals[i]);
        printf("%s\n", sval);
        free(sval);
    }
    return 0;
}
