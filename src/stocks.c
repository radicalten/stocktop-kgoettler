#include <curses.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "api.h"
#include "stocks.h"
#include "str.h"

#define MAXLEN 10

char ***create_stock_table(struct stock_data *stocks, int n)
{
    char *buf;
    char ***out = allocate_matrix(11, n, MAXLEN);
    for (int i = 0; i < n; i++)
    {
        snprintf(out[SYMBOL][i], MAXLEN, "%-8s", stocks[i].symbol);
        snprintf(out[OPEN][i], MAXLEN, "%8.2lf", stocks[i].open);
        snprintf(out[PRICE][i], MAXLEN, "%8.2lf", stocks[i].price);
        snprintf(out[CHANGE][i], MAXLEN, "%8.2lf", stocks[i].change);
        snprintf(out[CHANGE_PERC][i], MAXLEN, "%8.2lf", stocks[i].change_perc);
        buf = double_to_ss(stocks[i].volume);
        snprintf(out[VOLUME][i], MAXLEN, "%8s", buf);
        free(buf);
        buf = double_to_ss(stocks[i].volume_avg);
        snprintf(out[VOLUME_AVG][i], MAXLEN, "%8s", buf);
        free(buf);
        buf = double_to_ss(stocks[i].ebitda);
        snprintf(out[EBITDA][i], MAXLEN, "%8s", buf);
        free(buf);
        buf = double_to_ss(stocks[i].market_cap);
        snprintf(out[MARKET_CAP][i], MAXLEN, "%8s", buf);
        free(buf);
        snprintf(out[FIFTY_TWO_WEEK_LOW][i], MAXLEN, "%8.2lf", stocks[i].fifty_two_week_low);
        snprintf(out[FIFTY_TWO_WEEK_HIGH][i], MAXLEN, "%8.2lf", stocks[i].fifty_two_week_high);
    }
    return out;
}

void destroy_stock_table(char ***matrix, int n)
{
    deallocate_matrix(matrix, 11, n);
    return;
}

char ***allocate_matrix(int nrows, int ncols, int nchar) 
{
    int i, j;
    char ***matrix;
    /*  Allocate array of pointers  */
    matrix = malloc(nrows * sizeof(char**));
    if (matrix == NULL)
        return NULL;
    /*  Allocate column for each name  */
    for (i = 0; i < nrows; i++)
        matrix[i] = malloc(ncols * sizeof(char*));
    if (matrix[i-1] == NULL) 
        return NULL;
    for (i = 0; i < nrows; i++)
        for (j = 0; j < ncols; j++)
            matrix[i][j] = malloc(nchar * sizeof(char));
    if (matrix[i-1][j-1] == NULL)
        return NULL;
    return matrix;
}

void deallocate_matrix(char ***matrix, int nrows, int ncols)
{
    for (int i = 0; i < nrows; i++)
    {
        for (int j = 0; j < ncols; j++)
        {
            free(matrix[i][j]);
        }
        free(matrix[i]);
    }
    free(matrix);
}

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

void print_header(int row, int col)
{
    char msg[] = "STOCKTOP";
    mvprintw(row/2, (col-strlen(msg))/2, "%s", msg);
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
            "52WKLO",
            "52WKHI");
    return;
}

void print_stock(struct stock_data *stock, int line, int row, int col)
{
    char *volume, *volume_avg, *ebitda, *market_cap;
    if (stock->change > 0)
        attron(COLOR_PAIR(2));
    else if (stock->change < 0)
        attron(COLOR_PAIR(3));
    // string conversion
    volume = double_to_ss(stock->volume);
    volume_avg = double_to_ss(stock->volume_avg);
    ebitda = double_to_ss(stock->ebitda);
    market_cap = double_to_ss(stock->market_cap); 
    mvprintw(
            row/2 + line + 2,
            col/2 - 95/2,
            "%-5s %8.2f %8.2lf %8.2lf %8.2lf %8s %8s %8s %9s %8.2f %8.2f",
            stock->symbol,
            stock->open,
            stock->price,
            stock->change,
            stock->change_perc,
            volume,
            volume_avg,
            ebitda,
            market_cap,
            stock->fifty_two_week_low,
            stock->fifty_two_week_high ); 
    free(volume);
    free(volume_avg);
    free(ebitda);
    free(market_cap);
    if (stock->change > 0)
        attroff(COLOR_PAIR(2));
    else if (stock->change < 0)
        attroff(COLOR_PAIR(3));
    return;
}


void draw (struct stock_data *stocks, int n)
{
    int row, col;
    getmaxyx(stdscr, row, col);
    print_header(row, col);
    for (int i = 0; i < n; i++)
        print_stock(&stocks[i], i, row, col);
    return;
}
