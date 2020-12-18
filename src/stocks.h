#ifndef STOCKS_H_SEEN
#define STOCKS_H_SEEN

/**
 * Start curses session
 */
void start_curses(void);

/**
 * End/clean up curses session
 */
void end_curses(void);

/**
 * Print header line of table
 */
void print_header(int row, int col);

/**
 * Print a single stock to the table
 */
void print_stock(struct stock_data *stock, int line, int row, int col, int highlight);

/**
 * Draw the stocktop table
 */
void draw (struct stock_data *stocks, int n, int currow);
#endif
