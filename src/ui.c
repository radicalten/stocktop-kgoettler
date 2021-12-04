#include <curses.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>
#include <time.h>
#include "api.h"
#include "state.h"
#include "str.h"
#include "ui.h"

#define MIN_COLS 96

void start_curses(void)
{
    /* Setup ncurses */
    initscr();
    noecho();
    curs_set(0);
    keypad(stdscr, TRUE);
    start_color();
    /* Standard colors */
    init_pair(1, COLOR_BLUE, COLOR_BLACK);
    init_pair(2, COLOR_GREEN, COLOR_BLACK);
    init_pair(3, COLOR_RED, COLOR_BLACK);
    init_pair(4, COLOR_YELLOW, COLOR_BLACK);
    /* Highlighted colors */
    init_pair(5, COLOR_WHITE, COLOR_BLUE);
    init_pair(6, COLOR_BLACK, COLOR_GREEN);
    init_pair(7, COLOR_BLACK, COLOR_RED);
    init_pair(8, COLOR_BLACK, COLOR_YELLOW);
    return;
}

void end_curses(void)
{
    endwin();
    return;
}

int check_term_cols(int cols)
{
    if (cols < MIN_COLS)
        return 0;
    return 1;
}

int floor_div(int a, int b)
{
    int d = a / b;
    int r = a % b;  /* optimizes into single division. */
    return r ? (d - ((a < 0) ^ (b < 0))) : d;
}

int get_spacing(int cols)
{
    int cols_left;
    // Subtract the spacing of the columns themselves
    cols_left = cols - 6 - 80;
    return floor_div(cols_left, 10);
}

void set_color_on(double pchange, int highlight)
{
    if (pchange > 0.5)
    {
        if (!highlight)
            attron(COLOR_PAIR(2));
        else
            attron(COLOR_PAIR(6));
    }
    else if (pchange < -0.5)
    {
        if (!highlight)
            attron(COLOR_PAIR(3));
        else
            attron(COLOR_PAIR(7));
    }
    else
    {
        if (!highlight)
            attron(COLOR_PAIR(4));
        else
            attron(COLOR_PAIR(8));
    }
    return;
}

void set_color_off(double pchange, int highlight)
{
    if (pchange > 0.5)
    {
        if (!highlight)
            attroff(COLOR_PAIR(2));
        else
            attroff(COLOR_PAIR(6));
    }
    else if (pchange < -0.5)
    {
        if (!highlight)
            attroff(COLOR_PAIR(3));
        else
            attroff(COLOR_PAIR(7));
    }
    else
    {
        if (!highlight)
            attroff(COLOR_PAIR(4));
        else
            attroff(COLOR_PAIR(8));
    }
    return;
}

void set_color_error_on(int highlight)
{
    if (!highlight)
        attron(COLOR_PAIR(1));
    else
        attron(COLOR_PAIR(5));
    return;
}

void set_color_error_off(int highlight)
{
    if (!highlight)
        attroff(COLOR_PAIR(1));
    else
        attroff(COLOR_PAIR(5));
    return;
}

void print_width_error(int row, int col)
{
    char msg[] = "Terminal size is too small. Resize to show stocktop.";
    mvprintw(row/2, 0, "%s", msg);
    return;
}

void print_header(StockDataArray *data, int row, int col)
{
    int spacing = get_spacing(col);
    char msg[] = "STOCKTOP";
    char *refresh_time = malloc(40 * sizeof(char));
    StockDataArray_GetRefreshTimeStr(data, refresh_time);
    mvprintw(0, (col-strlen(msg))/2, "%s", msg);
    mvprintw(0 + 1, (col-strlen(refresh_time))/2, "%s", refresh_time);
    mvprintw(
            0 + 2,
            0,
            "%-5s%*s%8s%*s%8s%*s%8s%*s%8s%*s%8s%*s%8s%*s%8s%*s%8s%*s%8s%*s%8s",
            "",
            spacing,
            " ",
            "OPEN",
            spacing,
            " ",
            "PRICE",
            spacing,
            " ",
            "CHANGE",
            spacing,
            " ",
            "CHANGE%",
            spacing,
            " ",
            "VOL",
            spacing,
            " ",
            "VOLAVG",
            spacing,
            " ",
            "EBITDA",
            spacing,
            " ",
            "MARKETCAP",
            spacing,
            " ",
            "52WKLO",
            spacing,
            " ",
            "52WKHI");
    return;
}

void print_stock(StockData *stock, int line, int row, int col, int highlight)
{
    char *volume, *volume_avg, *ebitda, *market_cap;
    int spacing = get_spacing(col);
    // string conversion
    if (stock->error == 0)
    {
        set_color_on(stock->change_perc, highlight);
        volume = double_to_ss(stock->volume);
        volume_avg = double_to_ss(stock->volume_avg);
        ebitda = double_to_ss(stock->ebitda);
        market_cap = double_to_ss(stock->market_cap); 
        mvprintw(
                0 + line + 3,
                0,
                "%-5s%*s%8.2f%*s%8.2lf%*s%8.2lf%*s%8.2lf%*s%8s%*s%8s%*s%8s%*s%9s%*s%8.2f%*s%8.2f",
                stock->symbol,
                spacing,
                " ",
                stock->open,
                spacing,
                " ",
                stock->price,
                spacing,
                " ",
                stock->change,
                spacing,
                " ",
                stock->change_perc,
                spacing,
                " ",
                volume,
                spacing,
                " ",
                volume_avg,
                spacing,
                " ",
                ebitda,
                spacing,
                " ",
                market_cap,
                spacing,
                " ",
                stock->fifty_two_week_low,
                spacing,
                " ",
                stock->fifty_two_week_high );
        free(volume);
        free(volume_avg);
        free(ebitda);
        free(market_cap);
        set_color_off(stock->change_perc, highlight);
    }
    else
    {
        set_color_error_on(highlight);
        mvprintw(
                0 + line + 3,
                0,
                "%s is not a recognized stock symbol",
                stock->symbol);
        set_color_error_off(highlight);
    }
    return;
}

void draw (StockDataArray *stocks, StocktopState *state)
{
    int row, col;
    getmaxyx(stdscr, row, col);
    if (!check_term_cols(col))
    {
        print_width_error(row, col);
        return;
    }
    print_header(stocks, row, col);
    int i = 0;
    StockData *current = stocks->head;
    while (current)
    {
        print_stock(current, i, row, col, (i == state->currow) ? 1 : 0);
        current = current->next;
        i++;
    }
    return;
}

void update(StockDataArray *stocks, StocktopState *state)
{
    int row, col;
    int i = 0;
    getmaxyx(stdscr, row, col);
    StockData *current = stocks->head;
    while (current)
    {
        if (i == state->prevrow || i == state->currow)
        {
            print_stock(current, i, row, col, (i == state->currow) ? 1 : 0);
        }
        current = current->next;
        i++;
    }
    return;
}
