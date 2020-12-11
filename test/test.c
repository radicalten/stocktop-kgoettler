#include <check.h>
#include <curl/curl.h>
#include <json-c/json.h>
#include "api.h"

START_TEST (test_query)
{
    json_object *json;
    json_object **pjson = &json;
    int res;
    res = query(BASE_URL, pjson);
    ck_assert_int_eq(res, 0);
    json_object_put(json);
}
END_TEST

START_TEST (test_build_query)
{
    /* Inputs */
    char *SYMBOLS[3] = {"AAPL", "GOOG", "TSLA"};
    char *FIELDS[3] = {"shortName", "symbol", "marketState"};
    /* Build URL */
    char *url = build_endpoint(SYMBOLS, 3, FIELDS, 3);
    ck_assert_str_eq("https://query1.finance.yahoo.com/v7/finance/quote?lang=en-US&region=US&corsDomain=finance.yahoo.com&fields=shortName,symbol,marketState&symbols=AAPL,GOOG,TSLA", url);
    free(url);
}
END_TEST

START_TEST (test_fetch_stocks)
{
    struct stock_data *stocks;
    char *SYMBOLS[3] = {"AAPL", "GOOG", "WORK"};
    stocks = fetch_stocks(SYMBOLS, 3);
    for (int i = 0; i < 3; i++)
        printf("%s %s %lf %lf %lf\n", stocks[i].symbol, stocks[i].state, stocks[i].price, stocks[i].change, stocks[i].change_perc);
    free(stocks);
}
END_TEST


START_TEST (test_print_stocks)
{
    struct stock_data *stocks;
    char *SYMBOLS[3] = {"AAPL", "GOOG", "WORK"};
    stocks = fetch_stocks(SYMBOLS, 3);
    for (int i = 0; i < 3; i++) 
    {
        printf(
                "%-5s %-5s %8.2f %8.2lf %8.2lf %8.2lf %8.2lf %8.2lf %8.2lf %8.2lf %8.2lf\n",
                stocks[i].symbol,
                stocks[i].state,
                stocks[i].price,
                stocks[i].change,
                stocks[i].change_perc,
                stocks[i].volume,
                stocks[i].volume_avg,
                stocks[i].ebitda,
                stocks[i].market_cap,
                stocks[i].fifty_two_week_low,
                stocks[i].fifty_two_week_high );
    }
    free(stocks);
}
END_TEST

/*
START_TEST (test_parse_stocks)
{
    char *SYMBOLS[3] = {"AAPL", "GOOG", "TSLA"};
    char *FIELDS[3] = {"shortName", "symbol", "marketState"};
    int res;
    json_object *json;
    json_object **pjson = &json;
    char *url = build_endpoint(SYMBOLS, 3, FIELDS, 3);
    res = query(url, pjson); 
    struct stock_data stocks[3];
    parse_stocks(json, stocks, 3);

    for (int i = 0; i < 3; i++)
        printf("%s %s %lf %lf %lf\n", stocks[i].symbol, stocks[i].state, stocks[i].price, stocks[i].change, stocks[i].change_perc);
    free(url);
}
END_TEST
*/

int main(void)
{
    Suite *s1 = suite_create("API");
    TCase *tc1_1 = tcase_create("API");
    SRunner *sr = srunner_create(s1);
    int nf;

    suite_add_tcase(s1, tc1_1);
    tcase_add_test(tc1_1, test_query);
    tcase_add_test(tc1_1, test_build_query);
    tcase_add_test(tc1_1, test_fetch_stocks);
    tcase_add_test(tc1_1, test_print_stocks);

    srunner_run_all(sr, CK_ENV);
    nf = srunner_ntests_failed(sr);
    srunner_free(sr);
    return nf == 0 ? 0 : 1;
}
