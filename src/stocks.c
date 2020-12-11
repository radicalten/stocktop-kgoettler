#include <curses.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "api.h"
#include "str.h"

/**
 * Setup the curses session
 */
void start_curses(void)
{
    /* Setup ncurses */
    initscr();
    noecho();
    curs_set(0);
    keypad(stdscr, TRUE);
    start_color();
    init_pair(1, COLOR_WHITE, COLOR_BLUE);
    init_pair(2, COLOR_GREEN, COLOR_BLACK);
    init_pair(3, COLOR_RED, COLOR_BLACK);
    return;
}

/**
 * Clean up the curses session
 */
void end_curses(void)
{
    endwin();
    return;
}

/**
 * Sample message to draw on ncurses screen
 */
/*
void draw(void)
{
    char msg[] = "This is a test.";
    int row, col;
    attron(COLOR_PAIR(1));
    getmaxyx(stdscr, row, col);
    mvprintw(row/2, (col-strlen(msg))/2, "%s", msg);
    mvprintw(row-2, 0, "This screen has %d rows and %d columns\n", row, col);
    printw("Try resizing your window(if possible) and then run this program again");
    attroff(COLOR_PAIR(1));
    return;
}
*/

void draw (struct stock_data *stocks, int n)
{
    int row, col;
    char msg[] = "STOCKTOP";
    getmaxyx(stdscr, row, col);
    mvprintw(row/2, (col-strlen(msg))/2, "%s", msg);
    /* Print header */
    mvprintw(
            row/2 + 1,
            col/2 - 95/2,
            "%-5s %8s %8s %8s %8s %8s %8s %8s %8s %8s %8s",
            "",
            "OPEN",
            "PRICE", 
            "CHANGE", 
            "CHANGE%", 
            "VOL", 
            "VOLAVG",
            "EBITDA", 
            "MARKETCAP",
            "52WKHI",
            "52WKLO");
    for (int i = 0; i < n; i++)
    {
        if (stocks[i].change > 0)
            attron(COLOR_PAIR(2));
        else if (stocks[i].change < 0)
            attron(COLOR_PAIR(3));
        mvprintw(
                row/2 + i + 2,
                col/2 - 95/2,
                "%-5s %8.2f %8.2lf %8.2lf %8.2lf %8s %8s %8s %9s %8.2f %8.2f",
                stocks[i].symbol,
                stocks[i].open,
                stocks[i].price,
                stocks[i].change,
                stocks[i].change_perc,
                double_to_ss(stocks[i].volume),
                double_to_ss(stocks[i].volume_avg),
                double_to_ss(stocks[i].ebitda),
                double_to_ss(stocks[i].market_cap),
                stocks[i].fifty_two_week_low,
                stocks[i].fifty_two_week_high ); 
        if (stocks[i].change > 0)
            attroff(COLOR_PAIR(2));
        else if (stocks[i].change < 0)
            attroff(COLOR_PAIR(3));
    }
    return;
}

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
        draw(stocks, 3);
        /* Redraw */
    }
    end_curses();
    free(stocks);
    return 0;
}
