#pragma once
#include <string>

enum class OrderBookType { bid, ask, unknown, asksale, bidsale };

class OrderBookEntry {
public:

    OrderBookEntry() = default;
    OrderBookEntry(
        double _price,
        double _amount,
        std::string _timestamp,
        std::string _product,
        OrderBookType _orderType,
        std::string _username = "dataset"
    );
    ~OrderBookEntry() = default;
    static OrderBookType stringToOrderBookType(const std::string& s);
    static bool compareByTimestamp(const OrderBookEntry& e1, const OrderBookEntry& e2);

    static bool compareByPriceAsc(const OrderBookEntry& e1, const OrderBookEntry& e2);
    static bool compareByPriceDsc(const OrderBookEntry& e1, const OrderBookEntry& e2);

    double price;
    double amount;
    std::string timestamp;
    std::string product;
    OrderBookType orderType;
    std::string username;
};
