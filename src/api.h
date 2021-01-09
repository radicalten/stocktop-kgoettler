#ifndef API_H_SEEN
#define API_H_SEEN

#include <json-c/json.h>
#include <curl/curl.h>

#define BASE_URL "https://query1.finance.yahoo.com/v7/finance/quote?lang=en-US&region=US&corsDomain=finance.yahoo.com"

struct stock_array {
    struct stock_data *stocks;
    int length;
};

struct stock_data {
    char symbol[24];
    char state[24];
    double open;
    double price;
    double change;
    double change_perc;
    double volume;
    double volume_avg;
    double ebitda;
    double market_cap;
    double fifty_two_week_low;
    double fifty_two_week_high;
};

struct curl_fetch_st {
    char *payload;
    size_t size;
};

/**
 * Fetch stock data, given a list of stock symbols
 *
 * \param symbols - array of stock symbols
 * \param nsymbols - number of symbols passed
 */
struct stock_data* fetch_stocks(char **symbols, int nsymbols);

/**
 * Build full endpiont to use in cURL call
 *
 * \param nsymbols - number of stock symbols
 * \param symbols - pointer to strings containing stock symbols
 * \param nfields - number of fields to include in query
 * \param fields - point to strings containing fields to include in query
 */
char* build_endpoint(char **symbols, int nsymbols, char **fields, int nfields);

/**
 * Perform full query to Yahoo Finance API
 *
 * \param url - url (endpoint) built using build_endpoint()
 * \param json - pointer to a json_object
 */
int query(char *url, json_object **json);

/**
 * Parse a complete API request into an array of stock_data structs
 *
 * \param jobj - JSON result from cURL call
 * \param out - array of stock_data structs
 * \param len - number of stocks in JSON result (should match the length of out)
 */
void parse_stocks(json_object *jobj, struct stock_data *out, int len);


/**
 * Parse data from a single stock into a stock_data struct
 *
 * \param jobj - JSON result from cURL call
 * \param out - stock_data struct in which to place output data
 */
void parse_stock(json_object *jobj, struct stock_data *out);

/**
 * Callback for curl call
 */
size_t curl_callback (void *contents, size_t size, size_t nmemb, void *userp);

/**
 * Perform curl call
 */
CURLcode curl_fetch_url(CURL *ch, const char *url, struct curl_fetch_st *fetch);

#endif
