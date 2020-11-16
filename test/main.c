#include <check.h>
#include <curl/curl.h>
#include <json-c/json.h>
#include "api.h"

int main (void)
{
    /* Inputs */
    char *SYMBOLS[3] = {"AAPL", "GOOG", "TSLA"};
    char *FIELDS[13] = {"shortName", "symbol", "marketState", "marketState", "preMarketChange", "postMarketChange", "preMarketPrice", "preMarketChangePercent", "postMarketPrice", "postMarketChangePercent", "regularMarketPrice", "regularMarketChange", "regularMarketChangePercent"};
    
    /* Outputs */
    int res;
    json_object *json;
    json_object **pjson = &json;
    
    /* Build URL */
    char *url = build_endpoint(3, SYMBOLS, 13, FIELDS);
    
    /* Perform query */
    res = query(url, pjson); 
    
    /* Parse output */
    struct stock_data stocks[3];
    parse_stocks(json, stocks, 3);
    
    /* Print */
    for (int i = 0; i < 3; i++)
    {
        printf("%s %s %lf %lf %lf\n", stocks[i].symbol, stocks[i].state, stocks[i].price, stocks[i].change, stocks[i].change_perc);
    }
    free(url);
    return 0;
}
