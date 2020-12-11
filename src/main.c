#include <curses.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "api.h"
#include "stocks.h"
#include "str.h"

int main (void) 
{
    char key;
    start_curses();
    /* Get data */
    char *SYMBOLS[3] = {
        "AAPL",
        "GOOG",
        "TSLA"
    };
    struct stock_data * stocks = fetch_stocks(SYMBOLS, 3);
    draw(stocks, 3);
    while ((key = getch()) != 'q')
    {
        switch (key)
        {
            default:
                break;
        }
        clear();
        /* Redraw */
        draw(stocks, 3);
    }
    end_curses();
    free(stocks);
    return 0;
}
