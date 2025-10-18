
# Order Book Trading System

This project implements a cryptocurrency order book trading system in C++, following the curriculum from the [Object-Oriented Programming Specialization](https://www.coursera.org/specializations/object-oriented-programming-s12n) on Coursera.

## Overview

The system simulates a cryptocurrency exchange order book that processes buy (bid) and sell (ask) orders, matches them according to price-time priority, and executes trades. It demonstrates core object-oriented programming concepts including classes, encapsulation, and data structures.

## Features

- **Order Book Management**: Load and process trading data from CSV files
- **Order Matching Engine**: Match bids and asks using price-time priority algorithm
- **Market Statistics**: Calculate price statistics including median, average, spread, and price changes
- **Interactive Trading Interface**: Menu-driven system for placing orders and viewing market data
- **CSV Data Processing**: Tokenize and parse trading data with custom CSV reader

## Key Components

- `OrderBook`: Core order book functionality and matching engine
- `OrderBookEntry`: Individual order representation
- `CSVReader`: CSV file parsing and data tokenization
- `MerkelMain`: Main application interface and user interaction
- `Wallet`: Trading wallet management

## Matching Algorithm

The system implements a stateless batch matching algorithm:
- Lowest ask prices are processed first
- Highest matching bids get priority
- Trades execute at the ask price (lowest price paid)
- Partial order fills are supported
- Unmatched portions remain available for future matching

## Build and Run

```bash
cd merlel/
clang++ -std=c++20 *.cpp -o merklerex
./merklerex
```

## Pseudo Code
### Tokenise Pseudo Code
```
string csvLine = 'thing,thing2,thing3'    ## the string we are processing

string vector tokens    ## stores the tokens

char separator = ','    ## the hcaracter that seprates the tokes

int start, end  ## used to delineate the position of the tokens

start = csvLine.find_dirst_not_of(separator)
do
    end = next 'separator' after start
    if start == csvLine.length or start == end  ## nothing more to find
        break
    if end >= 0 ## we found the separator
        token = csvLine.substr(start, end - start)  ## start, substring length
    else    ## the last token
        token = csvLine.substr(start, csvLine.length - start)   ## end is invalid
    
    tokens.push_back(token) ## save the token
    start = end + 1 ## move past this token

while (end > 0) ## continue loop condition
```

### Matching Algorithm
Stateless batch simulation (like you have now)
- Just read from a file (CSV), sort/filter, compute trades.
- Orders don’t persist — you don’t track whether an order is “open”, “partial”, or “closed”.

Key features:
- The lowest ask is processed first.
- The highest bid that matches an ask is given priority over lower, matching
bids.
- The lowest price is paid - so if a bid is offering to pay more than an ask,
the bidder will only pay as much as the asker wants.
- Partial sales are allowed - if an ask and a bid match but the amounts do
not match, the largest possible amount is sold
- Partially matched bids or asks can be re-processed and matched against
further bids or asks

```
asks = orderbook.asks in this timeframe
bids = orderbook.bids in this timeframe

sales = []
sort asks lowest first
sort bids highest first

for ask in asks:
    for bid in bids:
        if bid.price >= ask.price # we have a match
            sale = new orderbookentry()
            sale.price = ask.price

            if bid.amount == ask.amount: # bid completely clears ask
                sale.amount = ask.amount
                sales.append(sale)
                bid.amount = 0 # make sure the bid is not processed again
                # can do no more with this ask
                # go onto the next ask
                break

            if bid.amount > ask.amount: # ask is completely gone slice the bid
                sale.amount = ask.amount
                sales.append(sale)
                # we adjust the bid in place
                # so it can be used to process the next ask
                bid.amount = bid.amount - ask.amount
                # ask is completely gone, so go to next ask
                break

            if bid.amount < ask.amount # bid is completely gone, slice the ask
                sale.amount = bid.amount
                sales.append(sale)
                # update the ask
                # and allow further bids to process the remaining amount
                ask.amount = ask.amount - bid.amount
                bid.amount = 0 # make sure the bid is not processed again

                # some ask remains so go to the next bid
                continue
return sales
```