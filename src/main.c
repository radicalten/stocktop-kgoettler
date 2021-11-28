#include <curses.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "api.h"
#include "stocks.h"
#include "rc.h"
#include "ui.h"
#include "str.h"

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
    int currow = 0;
    /* Get data */
    struct symbol_array *symbols = read_rcfile();
    int nsymbols = symbols->len;
    StockDataArray *data = StockDataArray_Create(symbols->symbols, nsymbols);
    StockDataArray_Query(data);
    /* Start interactive curses session */
    start_curses();
    draw(data, 0);
    while ((key = getch()) != 'q')
    {
        switch (key)
        {
            case ('k'):
            case (KEY_UP):
                currow = imax(currow - 1, 0);
                break;
            case ('j'):
            case (KEY_DOWN):
                currow = imin(currow + 1, nsymbols -1);
                break;
            case ('r'):
                StockDataArray_Query(data);
                // Update data
                break;
            default:
                break;
        }
        clear();
        /* Redraw */
        draw(data, currow);
    }
    end_curses();
    delete_symbol_array(symbols);
    StockDataArray_Delete(data);
    return 0;
}
