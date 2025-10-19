#pragma once
#include <string>
#include <map>
#include "OrderBookEntry.h"

class Wallet {
public:
    Wallet();
    /** inssert currency to the wallet */
    void insertCurrency(std::string type, double amount);
    /** remove currency from the wallet */
    bool removeCurrency(std::string type, double amount);
    /** check if the wallet contains this much currency or more */
    bool containsCurrency(std::string type, double amount);

    /** check if the wallet can fulfill this ask or bid order */
    bool canFulfillOrder(const OrderBookEntry& order);

    /** update the wallet assumes the order was made by the owner of the wallet */
    void processSale(const OrderBookEntry& sale);

    /** generate a string representation of the wallet */
    std::string toString();
private:
    std::map<std::string, double> currencies;
};