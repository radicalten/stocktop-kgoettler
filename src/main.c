#include <curses.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "api.h"
#include "stocks.h"
#include "str.h"

int main (void) 
{
    int key;
    int currow = 0;
    /* Get data */
    int nsymbols = 4;
    char *SYMBOLS[] = {
        "AAPL",
        "GOOG",
        "TSLA",
        "WORK"
    };
    struct stock_data * stocks = fetch_stocks(SYMBOLS, nsymbols);
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
            default:
                break;
        }
        clear();
        /* Redraw */
        draw(stocks, nsymbols, currow);
    }
    end_curses();
    free(stocks);
    return 0;
}
