#include <check.h>
#include <curl/curl.h>
#include <json-c/json.h>
#include "api.h"

int main (void)
{
    char *stocks[3] = {
        "AAPL",
        "GOOG",
        "TSLA"
    };
    struct stock_data *data = fetch_stocks(stocks, 3);
    for (int i = 0; i < 3; i++)
    {
        printf("%s %s %lf %lf %lf\n", data[i].symbol, data[i].state, data[i].price, data[i].change, data[i].change_perc);
    }
    free(data);
    return 0;
}
