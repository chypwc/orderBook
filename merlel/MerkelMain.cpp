
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
    wallet.insertCurrency("BTC", 10);  // initial balance of 10 BTC
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
    // 5 print wallet
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
}

void MerkelMain::enterAsk() // sell product
{
    std::string input;

    for (;;) {
        std::cout << "Make an ask — enter: product,price,amount (e.g. ETH/BTC,200,0.5)\n";

        // Read a full line, eating any leading whitespace/newlines.
        if (!std::getline(std::cin >> std::ws, input)) {
            std::cout << "Input stream closed.\n";
            break;
        }

        // Tokenize and basic shape check
        std::vector<std::string> tokens = CSVReader::tokenise(input, ',');
        if (tokens.size() != 3) {
            std::cout << "MerkelMain::enterAsk Bad input! Expected: product,price,amount\n";
            continue;
        }

        try {
            OrderBookEntry obe = CSVReader::stringsToOBE(
                tokens[0], // product
                tokens[1], // price
                tokens[2], // amount
                currentTime,
                OrderBookType::ask
            );
            obe.username = "simuser";

            if (!wallet.canFulfillOrder(obe)) {
                std::cout << "Wallet has insufficient funds to fulfill this ask.\n";
                continue;
            }

            std::cout << "Your ask order is — product: " << tokens[0]
                << "  price: " << tokens[1]
                << "  amount: " << tokens[2] << '\n';

            // Confirm loop
            for (;;) {
                std::cout << "Confirm? [y/n]: ";
                char c{};
                if (!(std::cin >> c)) {
                    std::cout << "Input stream closed.\n";
                    return;
                }
                // flush remainder of the confirmation line
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

                if (c == 'y' || c == 'Y') {
                    orderBook.insertOrder(obe);
                    std::cout << "Ask added to the order book.\n";
                    return; // done with enterAsk
                }
                if (c == 'n' || c == 'N') {
                    std::cout << "Okay, let's re-enter the order.\n";
                    break; // break confirm loop; go back to main while to re-enter
                }
                std::cout << "Please enter 'y' or 'n'.\n";
            }
        }
        catch (const std::exception& e) {
            std::cout << "MerkelMain::enterAsk Bad input! " << e.what() << '\n';
            continue;
        }
    } // end for
}

void MerkelMain::enterBid() // buy product
{
    std::string input;

    for (;;) {
        std::cout << "Make a bid — enter: product,price,amount (e.g. ETH/BTC,200,0.5)\n";

        // Read a full line, skipping any leading whitespace/newlines
        if (!std::getline(std::cin >> std::ws, input)) {
            std::cout << "Input stream closed.\n";
            break;
        }

        // Tokenize and basic shape check
        std::vector<std::string> tokens = CSVReader::tokenise(input, ',');
        if (tokens.size() != 3) {
            std::cout << "MerkelMain::enterBid Bad input! Expected: product,price,amount\n";
            continue;
        }

        try {
            OrderBookEntry obe = CSVReader::stringsToOBE(
                tokens[0],   // product
                tokens[1],   // price
                tokens[2],   // amount
                currentTime,
                OrderBookType::bid
            );
            obe.username = "simuser";

            if (!wallet.canFulfillOrder(obe)) {
                std::cout << "Wallet has insufficient funds to place this bid.\n";
                continue;
            }

            std::cout << "Your bid order is — product: " << tokens[0]
                << "  price: " << tokens[1]
                << "  amount: " << tokens[2] << '\n';

            // Confirmation loop
            for (;;) {
                std::cout << "Confirm? [y/n]: ";
                char c{};
                if (!(std::cin >> c)) {
                    std::cout << "Input stream closed.\n";
                    return;
                }
                // flush remainder of the confirmation line
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

                if (c == 'y' || c == 'Y') {
                    orderBook.insertOrder(obe);
                    std::cout << "Bid added to the order book.\n";
                    return; // done with enterBid
                }
                if (c == 'n' || c == 'N') {
                    std::cout << "Okay, let's re-enter the order.\n";
                    break; // back to outer for(;;) to re-enter
                }
                std::cout << "Please enter 'y' or 'n'.\n";
            }
        }
        catch (const std::exception& e) {
            std::cout << "MerkelMain::enterBid Bad input! " << e.what() << '\n';
            continue;
        }
    }
}

void MerkelMain::printWallet()
{
    std::cout << "Wallet" << std::endl;
    std::cout << wallet.toString() << std::endl;
}

void MerkelMain::gotoNextTimeframe()
{
    for (std::string& product : orderBook.getKnownProducts()) {
        std::vector<OrderBookEntry> sales = orderBook.matchAsksToBids(product, currentTime);
        std::cout << "Sales: " << sales.size() << std::endl;
        for (OrderBookEntry& sale : sales)
        {
            std::cout << "Sale - time: " << sale.timestamp
                << ", product: " << sale.product
                << ", price: " << sale.price
                << ", amount: " << sale.amount << std::endl;
            if (sale.username == "simuser") {
                // update the wallet
                wallet.processSale(sale);
            }
        }
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

