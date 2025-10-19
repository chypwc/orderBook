#include "Wallet.h"
#include <iostream>
#include "CSVReader.h"

Wallet::Wallet() {

}

/** inssert currency to the wallet */
void Wallet::insertCurrency(std::string type, double amount) {
    double balance;
    if (amount < 0) {
        throw std::invalid_argument("Wallet::insertCurrency Amount cannot be negative");
    }
    if (currencies.count(type) == 0) { // heck if the std::map currencies variable does not have any "type" currency
        balance = 0;
    }
    else {
        balance = currencies[type];
    }
    balance += amount;
    currencies[type] = balance;
}

/** remove currency from the wallet */
bool Wallet::removeCurrency(std::string type, double amount) {
    if (amount < 0) {
        throw std::invalid_argument("Wallet::removeCurrency Amount cannot be negative");
    }
    if (currencies.count(type) == 0) {
        std::cout << "Wallet::removeCurrency Wallet does not contain this currency" << std::endl;
        return true;
    }
    else {
        if (containsCurrency(type, amount)) {
            currencies[type] -= amount;
            return true;
        }
    }
    return false;
}

/** check if the wallet contains this much currency or more */
bool Wallet::containsCurrency(std::string type, double amount) {
    if (currencies.count(type) == 0) {
        return false;
    }
    return currencies[type] >= amount;
}

/** generate a string representation of the wallet */
std::string Wallet::toString() {
    std::string s;
    for (std::pair<std::string, double> pair : currencies) {
        std::string currency = pair.first;
        double amount = pair.second;
        s += currency + " : " + std::to_string(amount) + "\n";
    }
    return s;
}

bool Wallet::canFulfillOrder(const OrderBookEntry& order) {
    std::vector<std::string> currs = CSVReader::tokenise(order.product, '/');
    // ask/sell
    if (order.orderType == OrderBookType::ask) {
        double amount = order.amount;
        std::string currency = currs[0];
        std::cout << "Wallet::canFulfillOrder " << currency << " " << amount << std::endl;
        return containsCurrency(currency, amount);
    }
    // bid/buy
    if (order.orderType == OrderBookType::bid) {
        double amount = order.amount * order.price;
        std::string currency = currs[1];
        std::cout << "Wallet::canFulfillOrder " << currency << " " << amount << std::endl;
        return containsCurrency(currency, amount);
    }
    return false;
}

/** update the wallet assumes the order was made by the owner of the wallet */
void Wallet::processSale(const OrderBookEntry& sale) {
    std::vector<std::string> currs = CSVReader::tokenise(sale.product, '/');
    // ask/sell
    if (sale.orderType == OrderBookType::asksale) {
        double outgoingAmount = sale.amount;
        std::string outgoingCurrency = currs[0];
        double incomingAmount = sale.amount * sale.price;
        std::string incomingCurrency = currs[1];

        currencies[outgoingCurrency] -= outgoingAmount;
        currencies[incomingCurrency] += incomingAmount;
    }
    // bid/buy
    if (sale.orderType == OrderBookType::bidsale) {
        double outgoingAmount = sale.amount * sale.price;
        std::string outgoingCurrency = currs[1];
        double incomingAmount = sale.amount;
        std::string incomingCurrency = currs[0];

        currencies[outgoingCurrency] -= outgoingAmount;
        currencies[incomingCurrency] += incomingAmount;

    }
}