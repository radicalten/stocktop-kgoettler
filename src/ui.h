#ifndef UI_H_SEEN 
#define UI_H_SEEN 

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
void print_header(StockDataArray *data, int row, int col);

/**
 * Print a single stock to the table
 */
void print_stock(StockData *stock, int line, int row, int col, int highlight);

/**
 * Draw the stocktop table
 */
void draw (StockDataArray *stocks, int currow);
#endif
