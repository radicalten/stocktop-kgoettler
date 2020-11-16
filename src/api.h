#ifndef API_H 
#define API_H

#define BASE_URL "https://query1.finance.yahoo.com/v7/finance/quote?lang=en-US&region=US&corsDomain=finance.yahoo.com"

struct stock_data {
    char* symbol;
    char* state;
    double price;
    double change;
    double change_perc;
};

struct curl_fetch_st {
    char *payload;
    size_t size;
};

/**
 * Build full endpiont to use in cURL call
 *
 * \param nsymbols - number of stock symbols
 * \param symbols - pointer to strings containing stock symbols
 * \param nfields - number of fields to include in query
 * \param fields - point to strings containing fields to include in query
 */
char* build_endpoint(int nsymbols, char **symbols, int nfields, char **fields);

/**
 * Perform full query to Yahoo Finance API
 *
 * \param url - url (endpoint) built using build_endpoint()
 * \param json - pointer to a json_object
 */
int query(char *url, json_object **json);

void parse_stocks(json_object *jobj, struct stock_data *out, int len);
void parse_stock(json_object *jobj, struct stock_data *out);

/**
 * Prints all stocks to the terminal. Calls print_stock on all of the
 * stocks in JSON object.
 *
 * \param jobj - a json_object returned by the Yahoo Finance API
 */

/**
 * Prints a single stock to the terminal.
 *
 * \param jobj - pointer to a json_object
 */

/**
 * Callback for curl call
 */
size_t curl_callback (void *contents, size_t size, size_t nmemb, void *userp);

/**
 * Perform curl call
 */
CURLcode curl_fetch_url(CURL *ch, const char *url, struct curl_fetch_st *fetch);

#endif
