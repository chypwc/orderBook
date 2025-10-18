#pragma once

#include "OrderBookEntry.h"
#include "CSVReader.h"
#include <string>
#include <vector>

class OrderBook
{
public:
    /**  construct, reading a csv data file */
    OrderBook(std::string filename);

    /** return vector of all known products in the dataset*/
    std::vector<std::string> getKnownProducts();

    /** return vector of Orders according to the input parameters*/
    std::vector<OrderBookEntry> getOrders(
        OrderBookType type,
        std::string product,
        std::string timestamp);

    /** returns the earliest time in the orderbook */
    std::string getEarliestTime();

    /** returns the next time after the given timestamp in the orderbook
     * If there is no next timestamp, wrap around to the earliest time.
     * timestamp is assumed to be formatted as YYYY/MM/DD HH:MM:SS.
     */
    std::string getNextTime(std::string timestamp);

    /** Insert and sort and order to order book */
    void insertOrder(OrderBookEntry& order);

    /** Match and return the orders */
    std::vector<OrderBookEntry> matchAsksToBids(
        const std::string product, const std::string timestamp);

    /**
     * @brief Return the lowest price among entries.
     * @param entries Snapshot of orders.
     * @return Min price, or NaN if entries is empty.
     */
    static double getLowPrice(const std::vector<OrderBookEntry>& entries);
    /**
     * @brief Return the highest price among entries.
     * @param entries Snapshot of orders (e.g., all asks for a product and timestamp).
     * @return Max price, or NaN if entries is empty.
     */
    static double getHighPrice(const std::vector<OrderBookEntry>& entries);
    /**
     * @brief Return the spread (High - Low).
     * @param entries Snapshot of orders.
     * @return Spread, or NaN if entries is empty.
     */
    static double getPriceSpread(const std::vector<OrderBookEntry>& entries);
    /**
     * @brief Return the arithmetic mean of prices.
     * @param entries Snapshot of orders.
     * @return Mean price, or NaN if entries is empty.
     */
    static double getAveragePrice(const std::vector<OrderBookEntry>& entries);
    /**
     * @brief Return the median price (50th percentile), robust to outliers.
     * @param entries Snapshot of orders.
     * @return Median price, or NaN if entries is empty.
     */
    static double getMedianPrice(const std::vector<OrderBookEntry>& entries);
    /**
     * @brief Percentage change between two prices: 100 * (curr - prev) / prev.
     * @param prev_price Baseline price (e.g., previous median).
     * @param curr_price Current price (e.g., current median).
     * @return Percentage change, or NaN if prev is 0 or either input is non-finite.
     */
    static double getPriceChangePct(double prev_price, double curr_price);
    /**
     * @brief Percentage change between two snapshots, using their median prices.
     * @param prev Previous snapshot of orders.
     * @param cur  Current snapshot of orders.
     * @return Percentage change (median→median), or NaN if either snapshot is empty.
     */
    static double getPriceChangePct(const std::vector<OrderBookEntry>& prev,
        const std::vector<OrderBookEntry>& cur);
private:
    std::vector<OrderBookEntry> orders;
};