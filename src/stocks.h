#ifndef STOCKS_H_SEEN
#define STOCKS_H_SEEN

char ***create_stock_table(struct stock_data *stocks, int n);
void destroy_stock_table(char ***matrix, int n);
char ***allocate_matrix(int nrows, int ncols, int nchar);
void deallocate_matrix(char ***matrix, int nrows, int ncols);
#endif
