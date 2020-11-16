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

curl "https://query1.finance.yahoo.com/v7/finance/quote?lang=en-US&region=US&corsDomain=finance.yahoo.com&symbols=AAPL" | jq
#curl "https://query1.finance.yahoo.com/v7/finance/quote?lang=en-US&region=US&corsDomain=finance.yahoo.com&fields=shortName,symbol,marketState,regularMarketPrice,regularMarketChange,regularMarketChangePercent,preMarketPrice,preMarketChange,preMarketChangePercent,postMarketPrice,postMarketChange,postMarketChangePercent,ebitda,marketCap&symbols=AAPL"
