#!/bin/bash

function strcat { local IFS="$1"; shift; echo "$*"; }

URL="https://query1.finance.yahoo.com/v7/finance/quote?lang=en-US&region=US&corsDomain=finance.yahoo.com"
FIELDS=(
    "shortName"
    "symbol"
    "marketState"
    "regularMarketPrice"
    "regularMarketChange"
    "regularMarketChangePercent"
    "preMarketPrice"
    "preMarketChange"
    "preMarketChangePercent"
    "postMarketPrice"
    "postMarketChange"
    "postMarketChangePercent"
    "ebitda"
    "marketCap"
    "fiftyTwoWeekLow"
    "fiftyTwoWeekHigh"
)
SYMBOLS=(
    "AAPL"
    "GOOG"
    "TSLA"
)
endpoint="${URL}&fields=$(strcat "," "${FIELDS[@]}")&symbols=$(strcat "," ${SYMBOLS[@]})"
#echo $endpoint

#curl $endpoint | jq

curl "https://query1.finance.yahoo.com/v7/finance/quote?lang=en-US&region=US&corsDomain=finance.yahoo.com&f=l1c1va2xj1b4j4dyekjm3m4rr5p5p6s7o&symbols=AAPL" | jq
#curl "https://query1.finance.yahoo.com/v7/finance/quote?lang=en-US&region=US&corsDomain=finance.yahoo.com&fields=shortName,symbol,marketState,regularMarketPrice,regularMarketChange,regularMarketChangePercent,preMarketPrice,preMarketChange,preMarketChangePercent,postMarketPrice,postMarketChange,postMarketChangePercent,ebitda,marketCap&symbols=AAPL"
