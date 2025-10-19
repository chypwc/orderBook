#pragma once

#include "OrderBookEntry.h"
#include "OrderBook.h"
#include "Wallet.h"
#include <vector>


class MerkelMain {
public:
    MerkelMain();
    /* Call this to start the sim */
    void init();
private:
    void loadOrderBook();
    void printMenu();
    int getUserOption();
    void printHelp();
    void printMarketStats();
    void enterAsk();
    void enterBid();
    void printWallet();
    void gotoNextTimeframe();
    void processUserOption(int userOption);

    std::string currentTime;

    OrderBook orderBook{ "20200317.csv" };
    // OrderBook orderBook{ "test.csv" }; // test matching algorithm

    Wallet wallet;
};