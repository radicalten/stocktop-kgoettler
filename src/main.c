#include <curses.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "api.h"
#include "rc.h"
#include "ui.h"
#include "str.h"

int main (void) 
{
    int key;
    int currow = 0;
    /* Get data */
    struct symbol_array *symbols = read_rcfile();
    int nsymbols = symbols->len;
    StockData *stocks = fetch_stocks(symbols->symbols, nsymbols);
    /* Start interactive curses session */
    start_curses();
    draw(stocks, nsymbols, 0);
    while ((key = getch()) != 'q')
    {
        switch (key)
        {
            case ('k'):
            case (KEY_UP):
                currow -= 1;
                if (currow < 0)
                    currow = 0;
                break;
            case ('j'):
            case (KEY_DOWN):
                currow += 1;
                if (currow > (nsymbols-1))
                    currow = nsymbols - 1;
                break;
            case ('r'):
                free(stocks);
                stocks = fetch_stocks(symbols->symbols, nsymbols);
            default:
                break;
        }
        clear();
        /* Redraw */
        draw(stocks, nsymbols, currow);
    }
    end_curses();
    delete_symbol_array(symbols);
    free(stocks);
    return 0;
}
