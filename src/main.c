#include <curses.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "api.h"
#include "stocks.h"
#include "rc.h"
#include "ui.h"
#include "str.h"
#include "state.h"

int imin(int a, int b)
{
    if (a < b) 
    {
        return a;
    }
    return b;
}

int imax(int a, int b)
{
    if (a < b)
    {
        return b;
    }
    return a;
}

int main (void) 
{
    int key;
    StocktopState *state = StocktopState_Create();
    /* Get data */
    struct symbol_array *symbols = read_rcfile();
    int nsymbols = symbols->len;
    StockDataArray *data = StockDataArray_Create(symbols->symbols, nsymbols);
    StockDataArray_Query(data);
    /* Start interactive curses session */
    start_curses();
    draw(data, state);
    while ((key = getch()) != 'q')
    {
        switch (key)
        {
            case ('k'):
            case (KEY_UP):
                state->prevrow = state->currow;
                state->currow = imax(state->currow - 1, 0);
                update(data, state);
                refresh();
                break;
            case ('j'):
            case (KEY_DOWN):
                state->prevrow = state->currow;
                state->currow = imin(state->currow + 1, nsymbols - 1);
                update(data, state);
                refresh();
                break;
            case ('r'):
                StockDataArray_Query(data);
                clear();
                draw(data, state);
                // Update data
                break;
            default:
                break;
        }
        /* Redraw */
    }
    end_curses();
    delete_symbol_array(symbols);
    StockDataArray_Delete(data);
    StocktopState_Delete(state);
    return 0;
}
