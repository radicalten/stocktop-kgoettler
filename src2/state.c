#include <stdio.h>
#include <stdlib.h>
#include "state.h"

StocktopState *StocktopState_Create(void)
{
    StocktopState *out = malloc(sizeof(StocktopState));
    out->prevrow = 0;
    out->currow = 0;
    return out;
}

void StocktopState_Delete(StocktopState *state)
{
    free(state);
    return;
}

