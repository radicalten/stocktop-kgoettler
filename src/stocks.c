#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "stocks.h"

StockDataArray *StockDataArray_Create(char **symbols, int nsymbols)
{
    StockDataArray *out = malloc(sizeof(StockDataArray));
    out->head = malloc(sizeof(StockData));
    strcpy(out->head->symbol, symbols[0]);
    out->length = nsymbols;
    
    // Populate list
    StockData *current = out->head;
    StockData *next;
    for (int i = 1; i < nsymbols; i++)
    {
        // Create the next element
        next = malloc(sizeof(StockData));
        strcpy(next->symbol, symbols[i]);
        next->next = NULL;
        
        // Iterate
        current->next = next;
        current = next;
    }
    return out;
}

void StockDataArray_Delete(StockDataArray *data)
{
    StockData *next;
    while (data->head)
    {
        next = data->head->next;
        free(data->head);
        data->head = next;
    }
    free(data);
    return;
}

void StockDataArray_Append(StockDataArray *data, char *symbol)
{
    return;
}
