#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "api.h"

struct stock_data *fetch_stocks(char **symbols, int nsymbols)
{
    char *fields[13] = {
        "shortName",
        "symbol",
        "marketState",
        "marketState",
        "preMarketChange",
        "postMarketChange",
        "preMarketPrice",
        "preMarketChangePercent",
        "postMarketPrice",
        "postMarketChangePercent",
        "regularMarketPrice",
        "regularMarketChange",
        "regularMarketChangePercent"
    };

    /* Outputs */
    json_object *json;
    json_object **pjson = &json;

    /* Build URL and perform query */
    char *url = build_endpoint(symbols, nsymbols, fields, 13);
    query(url, pjson);
    
    /* Parse output */
    struct stock_data *stocks = malloc(sizeof(struct stock_data) * nsymbols);
    parse_stocks(json, stocks, nsymbols);
    json_object_put(json);
    
    /* Clean up */
    free(url);
    return stocks;
}

char* build_endpoint(char **symbols, int nsymbols, char **fields, int nfields)
{
    /* Calculate length of fields and symbols arguments */
    int arglen = strlen(BASE_URL);
    arglen += strlen("&fields=");
    for (int i = 0; i < nfields; i++) 
    {
        arglen += strlen(fields[i]);
    }
    arglen += (nfields);
    arglen += strlen("&symbols=");
    for (int i = 0; i < nsymbols; i++)
    {
        arglen += strlen(symbols[i]);
    }
    arglen += (nsymbols);
   
    /* Allocate enough memory for full endpoint, and fill */
    char *endpoint = malloc(sizeof(char) * (arglen));
    strcpy(endpoint, BASE_URL);
    /* Add fields to API endpoint */
    strcat(endpoint, "&fields=");
    for (int i = 0; i < nfields; i++)
    {
        strcat(endpoint, fields[i]);
        strcat(endpoint, ",");
    }
    endpoint[strlen(endpoint)-1] = '\0';
    /* Add symbols to API endpoint */
    strcat(endpoint, "&symbols=");
    for (int i = 0; i < nsymbols; i++)
    {
        strcat(endpoint, symbols[i]);
        strcat(endpoint, ",");
    }
    endpoint[strlen(endpoint)-1] = '\0';
    return endpoint;
}

void parse_stocks(json_object *jobj, struct stock_data *out, int len)
{
    json_object * jsub;
    jsub = json_object_object_get(json_object_object_get(jobj, "quoteResponse"), "result");
    for (int i = 0; i < len; i++)
        parse_stock(json_object_array_get_idx(jsub, i), &out[i]);
    return;
}

void parse_stock(json_object *jobj, struct stock_data *out)
{
    char *mstate, *symbol;
    double price, diff, percent, premc, postmc;
    symbol = json_object_get_string(
            json_object_object_get(jobj, "symbol"));
    mstate = json_object_get_string(
            json_object_object_get(jobj, "marketState"));
    premc = json_object_get_double(
            json_object_object_get(jobj, "preMarketChange"));
    postmc = json_object_get_double(
            json_object_object_get(jobj, "postMarketChange"));
    if ((strcmp(mstate, "PRE") == 0) && (premc != 0))
    {
        price = json_object_get_double(
                json_object_object_get(jobj, "preMarketPrice"));
        diff = premc;
        percent = json_object_get_double(
                json_object_object_get(jobj, "preMarketChangePercent"));
    }
    /*
    else if (~strcmp(mstate, "REGULAR") && (postmc != 0))
    {
        price = json_object_get_double(
                json_object_object_get(jobj, "postMarketPrice"));
        diff = postmc;
        percent = json_object_get_double(
                json_object_object_get(jobj, "postMarketChangePercent"));
    }
    */
    else
    {
        price = json_object_get_double(
                json_object_object_get(jobj, "regularMarketPrice"));
        diff = json_object_get_double(
                json_object_object_get(jobj, "regularMarketChange"));
        percent = json_object_get_double(
                json_object_object_get(jobj, "regularMarketChangePercent"));
    }
    strcpy(out->symbol, symbol);
    strcpy(out->state, mstate);
    /*
    out->symbol = symbol;
    out->state = mstate;
    */
    out->price = price;
    out->change = diff;
    out->change_perc = percent;
    out->open = json_object_get_double(
            json_object_object_get(jobj, "regularMarketPreviousClose"));
    out->volume = json_object_get_double(
            json_object_object_get(jobj, "regularMarketVolume"));
    out->volume_avg = json_object_get_double(
            json_object_object_get(jobj, "averageDailyVolume10Day"));
    out->ebitda = json_object_get_double(
            json_object_object_get(jobj, "ebitda"));
    out->market_cap = json_object_get_double(
            json_object_object_get(jobj, "marketCap"));
    out->fifty_two_week_low = json_object_get_double(
            json_object_object_get(jobj, "fiftyTwoWeekLow"));
    out->fifty_two_week_high = json_object_get_double(
            json_object_object_get(jobj, "fiftyTwoWeekHigh"));
    return;
}

int query(char *url, json_object **json)
{
    CURL *ch;
    CURLcode rcode;
    struct curl_fetch_st curl_fetch;
    struct curl_fetch_st *cf = &curl_fetch;
    struct curl_slist *headers = NULL;
    enum json_tokener_error jerr = json_tokener_success;

    /* init curl handle */
    if ((ch = curl_easy_init()) == NULL)
    {
        fprintf(stderr, "ERROR: Failed to create curl handle in fetch_session");
        return 2;
    }

    /* set content type */
    headers = curl_slist_append(headers, "Accept: application/json");
    headers = curl_slist_append(headers, "Content-Type: application/json");

    /* fetch page and capture return code */
    rcode = curl_fetch_url(ch, url, cf);

    /* cleanup curl */
    curl_easy_cleanup(ch);
    curl_slist_free_all(headers);

    /* check return code */
    if (rcode != CURLE_OK || cf->size < 1) 
    {
        fprintf(stderr, "ERROR: Failed to fetch url - curl said: %s",
            curl_easy_strerror(rcode));
        return 3;
    }

    /* check payload */
    if (cf->payload != NULL) 
    {
        *json = json_tokener_parse_verbose(cf->payload, &jerr);
        free(cf->payload);
    } 
    else 
    {
        fprintf(stderr, "ERROR: Failed to populate payload");
        free(cf->payload);
        return 4;
    }

    /* check error */
    if (jerr != json_tokener_success) 
    {
        fprintf(stderr, "ERROR: Failed to parse json string");
        json_object_put(*json);
        return 5;
    }
    return 0;
}

size_t curl_callback (void *contents, size_t size, size_t nmemb, void *userp)
{
    size_t realsize = size * nmemb;
    struct curl_fetch_st *p = (struct curl_fetch_st *) userp;

    /* expand buffer */
    p->payload = (char *) realloc(p->payload, p->size + realsize + 1);

    /* check buffer */
    if (p->payload == NULL) {
      fprintf(stderr, "ERROR: Failed to expand buffer in curl_callback");
      free(p->payload);
      return -1;
    }

    /* copy contents to buffer */
    memcpy(&(p->payload[p->size]), contents, realsize);

    /* set new buffer size */
    p->size += realsize;

    /* ensure null termination */
    p->payload[p->size] = 0;

    /* return size */
    return realsize;
}

CURLcode curl_fetch_url(CURL *ch, const char *url, struct curl_fetch_st *fetch) 
{
    CURLcode rcode;

    /* init payload */
    fetch->payload = (char *) calloc(1, sizeof(fetch->payload));

    /* check payload */
    if (fetch->payload == NULL) {
        /* log error */
        fprintf(stderr, "ERROR: Failed to allocate payload in curl_fetch_url");
        /* return error */
        return CURLE_FAILED_INIT;
    }

    /* init size */
    fetch->size = 0;

    /* set url to fetch */
    curl_easy_setopt(ch, CURLOPT_URL, url);

    /* set calback function */
    curl_easy_setopt(ch, CURLOPT_WRITEFUNCTION, curl_callback);

    /* pass fetch struct pointer */
    curl_easy_setopt(ch, CURLOPT_WRITEDATA, (void *) fetch);

    /* set default user agent */
    curl_easy_setopt(ch, CURLOPT_USERAGENT, "libcurl-agent/1.0");

    /* set timeout */
    curl_easy_setopt(ch, CURLOPT_TIMEOUT, 5);

    /* enable location redirects */
    curl_easy_setopt(ch, CURLOPT_FOLLOWLOCATION, 1);

    /* set maximum allowed redirects */
    curl_easy_setopt(ch, CURLOPT_MAXREDIRS, 1);

    /* fetch the url */
    rcode = curl_easy_perform(ch);

    return rcode;
}
