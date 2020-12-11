#ifndef STR_H_SEEN
#define STR_H_SEEN

enum {
    SYMBOL,
    OPEN,
    PRICE,
    CHANGE,
    CHANGE_PERC,
    VOLUME,
    VOLUME_AVG,
    EBITDA,
    MARKET_CAP,
    FIFTY_TWO_WEEK_LOW,
    FIFTY_TWO_WEEK_HIGH
};

char* double_to_ss(double x);

#endif
