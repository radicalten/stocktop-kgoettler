#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "stocks.h"

StockDataArray *StockDataArray_Create(char **symbols, int nsymbols)
{
    StockDataArray *out = malloc(sizeof(StockDataArray));
    out->length = nsymbols;
    out->stocks = malloc(sizeof(StockData) * nsymbols);
    for (int i = 0; i < nsymbols; i++)
    {
        strcpy(out->stocks[i].symbol, symbols[i]);
    }
    return out;
}

void StockDataArray_Delete(StockDataArray *data)
{
    free(data->stocks);
    free(data);
    return;
}

void StockDataArray_Add(StockDataArray *data, char *symbol)
{
    int cnt = data->length;
    data->stocks = realloc(data->stocks, sizeof(StockData) * (cnt + 1));
    data->length = cnt + 1;
    strcpy(data->stocks[cnt].symbol, symbol);
    return;
}
