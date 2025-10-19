#include "OrderBookEntry.h"
#include "MerkelMain.h"
#include "Wallet.h"
#include <iostream>

int main()
{
    MerkelMain app{};
    app.init();


    return 0;

    // Wallet wallet;

    // wallet.insertCurrency("USDT", 100);
    // std::cout << wallet.toString() << std::endl;
    // std::cout << wallet.containsCurrency("USDT", 50) << std::endl; // 1
    // std::cout << wallet.containsCurrency("USDT", 500) << std::endl; // 0
    // std::cout << wallet.containsCurrency("ETH", 10) << std::endl; // 0

    // wallet.removeCurrency("USDT", 50);
    // std::cout << wallet.toString() << std::endl;
    // wallet.removeCurrency("USDT", 500);
    // std::cout << wallet.toString() << std::endl;
    // wallet.removeCurrency("USDT", -100);
    // std::cout << wallet.toString() << std::endl;

    // wallet.insertCurrency("BTC", 10);
    // std::cout << wallet.containsCurrency("BTC", 5) << std::endl; // 1
    // std::cout << wallet.containsCurrency("BTC", 15) << std::endl; // 0
    // std::cout << wallet.containsCurrency("ETH", 10) << std::endl; // 0
    // std::cout << wallet.toString() << std::endl;

    // wallet.removeCurrency("DOGE", 100); // does not contain this currency

    // OrderBookEntry order1{ 1000.0, 0.02, "2020/03/17 17:01:24.884492",  "BTC/USDT", OrderBookType::bid };
    // std::cout << wallet.canFulfillOrder(order1) << std::endl; // 1
    // order1 = { 1000.0, 1, "2020/03/17 17:01:24.884492",  "BTC/USDT", OrderBookType::bid };
    // std::cout << wallet.canFulfillOrder(order1) << std::endl; // 0

    // order1 = { 100.0, 100, "2020/03/17 17:01:24.884492",  "BTC/USDT", OrderBookType::ask };
    // std::cout << wallet.canFulfillOrder(order1) << std::endl; // 0
    // order1 = { 100.0, 1, "2020/03/17 17:01:24.884492",  "BTC/USDT", OrderBookType::ask };
    // std::cout << wallet.canFulfillOrder(order1) << std::endl; // 1


}


