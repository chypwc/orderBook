#include "OrderBookEntry.h"
#include <string>

OrderBookEntry::OrderBookEntry(double _price,
    double _amount,
    std::string _timestamp,
    std::string _product,
    OrderBookType _orderType)
    : price{ _price },
    amount{ _amount },
    timestamp{ _timestamp },
    product{ _product },
    orderType{ _orderType } {
}

OrderBookType OrderBookEntry::stringToOrderBookType(const std::string& s) {
    if (s == "bid") {
        return OrderBookType::bid;
    }
    if (s == "ask") {
        return OrderBookType::ask;
    }
    return OrderBookType::unknown;
}

bool OrderBookEntry::compareByTimestamp(const OrderBookEntry& e1, const OrderBookEntry& e2) {
    return e1.timestamp < e2.timestamp; // e1 is earlier, right order
}

bool OrderBookEntry::compareByPriceAsc(const OrderBookEntry& e1, const OrderBookEntry& e2) {
    return e1.price < e2.price;
}
bool OrderBookEntry::compareByPriceDsc(const OrderBookEntry& e1, const OrderBookEntry& e2) {
    return e1.price > e2.price;
}