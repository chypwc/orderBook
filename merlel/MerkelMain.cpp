
#include "MerkelMain.h"
#include "OrderBookEntry.h"
#include "CSVReader.h"
#include <iostream>
#include <vector>
#include <string>
#include <map>

MerkelMain::MerkelMain() {}

void MerkelMain::init()
{
    // the simulation begins at the earliest available market data point in the file.
    currentTime = orderBook.getEarliestTime();
    int input;
    while (true)
    {
        printMenu();
        try {
            input = getUserOption();
        }
        catch (std::exception& e) {
            continue;
        }
        processUserOption(input);
    }
}


void MerkelMain::printMenu()
{
    std::cout << "=============================" << std::endl;
    // 1 print help
    std::cout << "1: Print help" << std::endl;
    // 2 print exchange stats
    std::cout << "2: Print exchange stats" << std::endl;
    // 3 make an offer
    std::cout << "3: Make an offer" << std::endl;
    // 4: Make a bid
    std::cout << "4: Make a bid" << std::endl;
    // 5 print walletj
    std::cout << "5: Print wallet" << std::endl;
    // 6 continue
    std::cout << "6: Continue" << std::endl;
    std::cout << "=============================" << std::endl;

    std::cout << "Current time is: " << currentTime << std::endl;
}

int MerkelMain::getUserOption()
{
    int userOption;
    std::string input;
    std::cout << "Enter your choice (1-6): ";
    try {
        std::getline(std::cin, input);
        userOption = std::stoi(input);
        std::cout << "You chose: " << userOption << std::endl;
    }
    catch (std::exception& e) {
        std::cout << "MerkelMain::getUserOption Invalid input. Please enter a number (1 - 6)." << std::endl;
        throw;
    }
    return userOption;
}

void MerkelMain::printHelp()
{
    std::cout << "Help - your aim is to make money. Analyse the market and make bids and offers." << std::endl;
}

void MerkelMain::printMarketStats()
{
    for (const std::string& p : orderBook.getKnownProducts()) {
        std::cout << "--------------------------------" << std::endl;
        std::cout << "Product: " << p << std::endl;
        std::vector <OrderBookEntry> entries = orderBook.getOrders(OrderBookType::ask,
            p,
            currentTime
        );
        std::cout << "Asks seen: " << entries.size() << std::endl;
        std::cout << "Max ask: " << OrderBook::getHighPrice(entries) << std::endl;
        std::cout << "Min ask: " << OrderBook::getLowPrice(entries) << std::endl;
        std::cout << "Spread: " << OrderBook::getPriceSpread(entries) << std::endl;
        std::cout << "Median ask: " << OrderBook::getMedianPrice(entries) << std::endl;

        entries = orderBook.getOrders(OrderBookType::bid,
            p,
            currentTime
        );
        std::cout << "Bids seen: " << entries.size() << std::endl;
        std::cout << "Max bid: " << OrderBook::getHighPrice(entries) << std::endl;
        std::cout << "Min bid: " << OrderBook::getLowPrice(entries) << std::endl;
        std::cout << "Spread: " << OrderBook::getPriceSpread(entries) << std::endl;
        std::cout << "Median bid: " << OrderBook::getMedianPrice(entries) << std::endl;
    }

    // Example: compute stats for a product/timeframe you queried with getOrders(...)
    // auto entries_t0 = orderBook.getOrders(OrderBookType::ask, "BTC/USDT", "2020/03/17 17:01:50.116610");
    // auto entries_t1 = orderBook.getOrders(OrderBookType::ask, "BTC/USDT", "2020/03/17 17:01:55.120438");

    // double med0 = OrderBook::getMedianPrice(entries_t0);
    // double med1 = OrderBook::getMedianPrice(entries_t1);
    // double pct = OrderBook::getPriceChangePct(entries_t0, entries_t1);
    // std::cout << "Median ask price at t0: " << med0 << std::endl;
    // std::cout << "Median ask price at t1: " << med1 << std::endl;
    // std::cout << "Price change percentage: " << pct << std::endl;

    // std::cout << "OrderBook contains : " << orders.size() << " entries" << std::endl;

    // unsigned int bids{ 0 };
    // unsigned int asks{ 0 };
    // for (const OrderBookEntry& order : orders)
    // {
    //     if (order.orderType == OrderBookType::bid)
    //     {
    //         ++bids;
    //     }
    //     if (order.orderType == OrderBookType::ask)
    //     {
    //         ++asks;
    //     }
    // }

    // std::cout << "Orders: " << bids << " bids and " << asks << " asks" << std::endl;
}

void MerkelMain::enterAsk() // sell product 
{
    std::string input;
    std::vector<std::string> tokens;
    while (true) {
        std::cout << "Make an ask - enter the amount: product,price,amount, eg ETH/BTC,200,0.5" << std::endl;
        // clean end of line character from cin buffer
        if (!std::getline(std::cin >> std::ws, input)) break; // eat any leading newline/space

        tokens = CSVReader::tokenise(input, ',');
        if (tokens.size() != 3)
        {
            std::cout << "Bad input! Please follow the format: product,price,amount" << std::endl;
            continue;
        }
        else {
            try {
                OrderBookEntry obe = CSVReader::stringsToOBE(
                    tokens[0], // product
                    tokens[1], // price
                    tokens[2], // amount 
                    currentTime,
                    OrderBookType::ask
                );
                orderBook.insertOrder(obe);
            }
            catch (const std::exception& e) {
                std::cout << "MerkelMain::enterAsk Bad input! " << e.what() << std::endl;
                continue;
            }
        }
        std::cout << "Your ask order is - product: " << tokens[0]
            << " price:" << tokens[1]
            << " amount: " << tokens[2] << std::endl;
        std::cout << "Enter 'y' to confirm or 'n' to re-enter: ";
        if (std::cin.get() == 'y')
        {
            std::cout << "Ask added to the order book." << std::endl;
            break;
        }
    } // end of while 
}

void MerkelMain::enterBid() // bury product
{
    std::string input;
    std::vector<std::string> tokens;
    while (true) {
        std::cout << "Make a bid - enter the amount: product,price,amount, eg ETH/BTC,200,0.5" << std::endl;
        // clean end of line character from cin buffer
        if (!std::getline(std::cin >> std::ws, input)) break; // eat any leading newline/space

        tokens = CSVReader::tokenise(input, ',');
        if (tokens.size() != 3)
        {
            std::cout << "Bad input! Please follow the format: product,price,amount" << std::endl;
            continue;
        }
        else {
            try {
                OrderBookEntry obe = CSVReader::stringsToOBE(
                    tokens[0], // product
                    tokens[1], // price
                    tokens[2], // amount 
                    currentTime,
                    OrderBookType::bid
                );
                orderBook.insertOrder(obe);
            }
            catch (const std::exception& e) {
                std::cout << "MerkelMain::enterAsk Bad input! " << e.what() << std::endl;
                continue;
            }
        }
        std::cout << "Your ask order is - product: " << tokens[0]
            << " price:" << tokens[1]
            << " amount: " << tokens[2] << std::endl;
        std::cout << "Enter 'y' to confirm or 'n' to re-enter: ";
        if (std::cin.get() == 'y')
        {
            std::cout << "Ask added to the order book." << std::endl;
            break;
        }
    } // end of while 
}

void MerkelMain::printWallet()
{
    std::cout << "Wallet" << std::endl;
}

void MerkelMain::gotoNextTimeframe()
{
    // Test matchAsksToBids
    std::vector<OrderBookEntry> sales = orderBook.matchAsksToBids("ETH/BTC", currentTime);
    std::cout << "Sales: " << sales.size() << std::endl;
    for (OrderBookEntry& e : sales)
    {
        std::cout << "Sale - time: " << e.timestamp
            << ", product: " << e.product
            << ", price: " << e.price
            << ", amount: " << e.amount << std::endl;
    }

    currentTime = orderBook.getNextTime(currentTime);
    std::cout << "Automatically advances to the next timestamp: " << currentTime << std::endl;
}

void MerkelMain::processUserOption(int userOption)
{
    static const std::map<int, void (MerkelMain::*)()> menu{
        {1, &MerkelMain::printHelp},
        {2, &MerkelMain::printMarketStats},
        {3, &MerkelMain::enterAsk},
        {4, &MerkelMain::enterBid},
        {5, &MerkelMain::printWallet},
        {6, &MerkelMain::gotoNextTimeframe}
    };
    auto it = menu.find(userOption);
    if (it != menu.end())
    {
        // it->second is a pointer to member function of MerkelMain with type: void (MerkelMain::*)()
        // this is a MerkelMain* pointer
        // ->* is pointer-to-member operator for pointer "this"
        (this->*(it->second))();
        return;
    }
    else
    {
        std::cout << "Invalid choice" << std::endl;
    }
}

