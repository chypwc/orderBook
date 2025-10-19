#include "OrderBook.h"
#include <map>
#include <algorithm>

/* construct, reading a csv data file*/
OrderBook::OrderBook(std::string filename) {
    orders = CSVReader::readCSV(filename);
}

/* return vector of all known products in the dataset*/
std::vector<std::string> OrderBook::getKnownProducts() {

    std::vector<std::string> products;
    std::map<std::string, bool> prodMap;
    for (OrderBookEntry& e : orders) {
        prodMap[e.product] = true;
    }

    for (const auto& e : prodMap) {
        products.push_back(e.first);
    }

    return products;
}


/* return vector of Orders according to the input parameters*/
std::vector<OrderBookEntry> OrderBook::getOrders(OrderBookType type,
    std::string product,
    std::string timestamp) {
    std::vector<OrderBookEntry> orders_sub;
    for (OrderBookEntry& e : orders) {
        if (e.orderType == type && e.product == product && e.timestamp == timestamp) {
            orders_sub.push_back(e);
        }
    }
    return orders_sub;
}

std::string OrderBook::getEarliestTime() {
    return orders.at(0).timestamp;
}

std::string OrderBook::getNextTime(std::string timestamp) {
    // Assume orders are sorted by timestamp
    std::string next_timestamp = "";
    for (OrderBookEntry& e : orders) {
        if (e.timestamp > timestamp) {
            next_timestamp = e.timestamp;
            break;
        }
    }
    // wrap around to the first timeframe
    if (next_timestamp == "") {
        next_timestamp = orders.at(0).timestamp;
    }
    return next_timestamp;
}


void OrderBook::insertOrder(OrderBookEntry& order) {
    orders.push_back(order);
    std::sort(orders.begin(), orders.end(), OrderBookEntry::compareByTimestamp);
}

std::vector<OrderBookEntry>  OrderBook::matchAsksToBids(
    const std::string product, const std::string timestamp) {

    // asks = orderbook.asks in this timeframe
    std::vector<OrderBookEntry> asks = getOrders(OrderBookType::ask, product, timestamp);
    // bids = orderbook.bids in this timeframe
    std::vector<OrderBookEntry> bids = getOrders(OrderBookType::bid, product, timestamp);

    std::vector<OrderBookEntry> sales;
    // sort asks lowest first
    std::sort(asks.begin(), asks.end(), OrderBookEntry::compareByPriceAsc);
    // sort bids highest first
    std::sort(bids.begin(), bids.end(), OrderBookEntry::compareByPriceDsc);

    for (OrderBookEntry& ask : asks) {
        for (OrderBookEntry& bid : bids) {
            if (bid.price >= ask.price) {
                // sale.price = ask.price;
                OrderBookEntry sale{ ask.price, 0, timestamp, product, OrderBookType::asksale };

                // sale came from bid
                if (bid.username == "simuser") {
                    sale.orderType = OrderBookType::bidsale;
                    sale.username = "simuser";
                }
                // sale came from ask
                if (ask.username == "simuser") {
                    sale.orderType = OrderBookType::asksale;
                    sale.username = "simuser";
                }

                if (bid.amount == ask.amount) {
                    sale.amount = ask.amount;
                    sales.push_back(sale);
                    bid.amount = 0;
                    // No need to set ask.amount = 0, because the ask loop moves forward.
                    break; // go onto the next ask
                }
                if (bid.amount > ask.amount) {
                    sale.amount = ask.amount;
                    sales.push_back(sale);
                    bid.amount = bid.amount - ask.amount;
                    break; // go onto the next ask
                }
                if (bid.amount < ask.amount && bid.amount > 0) {
                    sale.amount = bid.amount;
                    sales.push_back(sale);
                    ask.amount = ask.amount - bid.amount;
                    bid.amount = 0;
                    continue; // # some ask remains so go to the next bid
                }
            }
        } // end bid for loop
    } // end ask for loop
    return sales;
}

/**
 * @brief Return the lowest price among entries.
 * @param entries Snapshot of orders.
 * @return Min price, or NaN if entries is empty.
 */
double OrderBook::getLowPrice(const std::vector<OrderBookEntry>& entries) {
    if (entries.empty()) {
        return 0.0;
    }
    double min{ entries.at(0).price };
    for (const OrderBookEntry& order : entries) {
        if (order.price < min) {
            min = order.price;
        }
    }
    return min;
}

/**
 * @brief Return the highest price among entries.
 * @param entries Snapshot of orders (e.g., all asks for a product and timestamp).
 * @return Max price, or NaN if entries is empty.
 */
double OrderBook::getHighPrice(const std::vector<OrderBookEntry>& entries) {
    if (entries.empty()) {
        return 0.0;
    }
    double max{ entries.at(0).price };
    for (const OrderBookEntry& order : entries) {
        if (order.price > max) {
            max = order.price;
        }
    }
    return max;
}

/**
 * @brief Return the spread (High - Low).
 * @param entries Snapshot of orders.
 * @return Spread, or NaN if entries is empty.
 */
double OrderBook::getPriceSpread(const std::vector<OrderBookEntry>& entries) {
    return getHighPrice(entries) - getLowPrice(entries);
}

/**
 * @brief Return the arithmetic mean of prices.
 * @param entries Snapshot of orders.
 * @return Mean price, or NaN if entries is empty.
 */
double OrderBook::getAveragePrice(const std::vector<OrderBookEntry>& entries) {
    if (entries.empty()) {
        return 0.0;
    }
    double sum{ 0.0 };
    int n{ 0 };
    for (const OrderBookEntry& order : entries) {
        sum += order.price;
    }
    return sum / entries.size();
}

/**
 * @brief Return the median price (50th percentile), robust to outliers.
 * @param entries Snapshot of orders.
 * @return Median price, or NaN if entries is empty.
 */
double OrderBook::getMedianPrice(const std::vector<OrderBookEntry>& entries) {

    // Copy prices to a small buffer to avoid reordering entries
    std::vector<double> prices;
    prices.reserve(entries.size());
    for (const OrderBookEntry& e : entries) prices.push_back(e.price);

    const size_t n = prices.size();
    const size_t mid = n / 2;

    // Use nth_element to get median in O(n) expected time (no full sort needed)
    std::nth_element(prices.begin(), prices.begin() + mid, prices.end());
    double med_hi = prices[mid];

    if (n % 2 == 1) {
        // odd: middle element
        return med_hi;
    }
    else {
        // even: need the lower middle as well
        std::nth_element(prices.begin(), prices.begin() + (mid - 1), prices.end());
        double med_lo = prices[mid - 1];
        return 0.5 * (med_lo + med_hi);
    }
}

/**
     * @brief Percentage change between two prices: 100 * (curr - prev) / prev.
     * @param prev_price Baseline price (e.g., previous median).
     * @param curr_price Current price (e.g., current median).
     * @return Percentage change, or NaN if prev is 0 or either input is non-finite.
     */
double OrderBook::getPriceChangePct(double prev_price, double curr_price) {
    return 100.0 * (curr_price - prev_price) / prev_price;
}

/**
 * @brief Percentage change between two snapshots, using their median prices.
 * @param prev Previous snapshot of orders.
 * @param cur  Current snapshot of orders.
 * @return Percentage change (median→median), or NaN if either snapshot is empty.
 */
double OrderBook::getPriceChangePct(const std::vector<OrderBookEntry>& prev,
    const std::vector<OrderBookEntry>& cur) {
    const double m_prev = getMedianPrice(prev);
    const double m_cur = getMedianPrice(cur);
    return getPriceChangePct(m_prev, m_cur);
}