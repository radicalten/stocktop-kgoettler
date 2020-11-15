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
    char *SYMBOLS[3] = {"AAPL", "GOOG", "TSLA"};
    char *FIELDS[3] = {"shortName", "symbol", "marketState"};
    char *url = build_endpoint(3, SYMBOLS, 3, FIELDS);
    ck_assert_str_eq("https://query1.finance.yahoo.com/v7/finance/quote?lang=en-US&region=US&corsDomain=finance.yahoo.com&fields=shortName,symbol,marketState&symbols=AAPL,GOOG,TSLA", url);
    free(url);
}
END_TEST


int main(void)
{
    Suite *s1 = suite_create("API");
    TCase *tc1_1 = tcase_create("API");
    SRunner *sr = srunner_create(s1);
    int nf;

    suite_add_tcase(s1, tc1_1);
    tcase_add_test(tc1_1, test_query);
    tcase_add_test(tc1_1, test_build_query);

    srunner_run_all(sr, CK_ENV);
    nf = srunner_ntests_failed(sr);
    srunner_free(sr);
    return nf == 0 ? 0 : 1;
}
