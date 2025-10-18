#pragma once

#include "OrderBookEntry.h"
#include <vector>
#include <string>

class CSVReader {
public:
    CSVReader();
    static std::vector<OrderBookEntry> readCSV(const std::string& filename);
    static std::vector<std::string> tokenise(const std::string& csvLine, char separator);
    static OrderBookEntry stringsToOBE(
        const std::string& productString,
        const std::string& priceString,
        const std::string& amountString,
        const std::string& timestamp,
        const OrderBookType& orderType);
private:
    static OrderBookEntry stringsToOBE(const std::vector<std::string>& tokens);
};