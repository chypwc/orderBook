#include "CSVReader.h"
#include <iostream>
#include <fstream>

CSVReader::CSVReader() {}

std::vector<OrderBookEntry> CSVReader::readCSV(const std::string& filename) {
    std::vector<OrderBookEntry> entries;
    std::ifstream csvFile{ filename };
    std::string line;
    std::vector<std::string> tokens;

    if (csvFile.is_open()) {
        std::cout << "File opened " << std::endl;

        while (std::getline(csvFile, line)) {   // get line from csvFile and store in line
            try {
                tokens = tokenise(line, ',');
                entries.push_back(stringsToOBE(tokens));
            }
            catch (const std::exception& e) {
                std::cout << "CSVReader::readCSV bad data " << std::endl;
                continue;
            }
        } // end of while
        csvFile.close();
    }
    else {
        std::cout << "File not opened " << filename << std::endl;
    }
    std::cout << "CSVReader::readCSV read " << entries.size() << " entries" << std::endl;

    return entries;
}

std::vector<std::string> CSVReader::tokenise(const std::string& csvLine, char separator) {

    std::vector<std::string> tokens;
    std::string::size_type start{ 0 }, end;  // used to delineate the position of the tokens
    std::string token;

    start = csvLine.find_first_not_of(separator, 0); // start from 0

    do {
        end = csvLine.find(separator, start);
        if (start == csvLine.length() || start == end) break;   // nothing more to find
        else if (end >= 0) token = csvLine.substr(start, end - start);  // start, substring length
        else token = csvLine.substr(start, csvLine.length() - start);  // the last token
        tokens.push_back(token);
        start = end + 1;
    } while (end != std::string::npos); // found separator
    // std::string::npos = “not a valid position” = usually means search failed.

    return tokens;
}

OrderBookEntry CSVReader::stringsToOBE(const std::vector<std::string>& tokens) {
    double price, amount;

    // Basic check: correct number of tokens
    if (tokens.size() != 5) {
        std::cout << "Incorrect number of fields/tokens. Bad line." << std::endl;
        throw std::exception{};
    }
    // Convert tokens into correct data types
    try {
        price = std::stod(tokens[3]);
        amount = std::stod(tokens[4]);
    }
    catch (std::exception& e) {
        std::cout << e.what() << std::endl;
        std::cout << "Bad float! " << tokens[3] << std::endl;
        std::cout << "Bad float! " << tokens[4] << std::endl;
        throw;
    }

    OrderBookEntry obe{ price, amount,
                        tokens[0], tokens[1], // timestamp, product
                        OrderBookEntry::stringToOrderBookType(tokens[2]) }; // bid/ask
    return obe;
}

OrderBookEntry CSVReader::stringsToOBE(
    const std::string& productString,
    const std::string& priceString,
    const std::string& amountString,
    const std::string& timestamp,
    const OrderBookType& orderType) {

    double price, amount;
    // Convert tokens into correct data types
    try {
        price = std::stod(priceString);
        amount = std::stod(amountString);
    }
    catch (std::exception& e) {
        std::cout << e.what() << std::endl;
        std::cout << "CSVReader::stringsToOBE Bad float! " << priceString << std::endl;
        std::cout << "CSVReader::stringsToOBE Bad float! " << amountString << std::endl;
        throw;
    }
    OrderBookEntry obe{ price, amount,
                        timestamp, productString, // timestamp, product
                        orderType }; // bid/ask
    return obe;
}