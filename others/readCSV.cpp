
#include <iostream>
# include <string>
#include <vector>
#include <fstream>

enum class OrderBookType { bid, ask };

std::vector<std::string> tokenise(const std::string& csvLine, char separator) {

    std::vector<std::string> tokens;
    std::string::size_type start{ 0 }, end;  // used to delineate the position of the tokens
    std::string token;

    // start = csvLine.find_dirst_not_of(separator)
    start = csvLine.find_first_not_of(separator, 0); // start from 0

    // do
    //     end = next 'separator' after start
    //     if start == csvLine.length or start == end  ## nothing more to find
    //         break
    //     if end >= 0 ## we found the separator
    //         token = csvLine.substr(start, end - start)  ## start, substring length
    //     else    ## the last token
    //         token = csvLine.substr(start, csvLine.length - start)   ## end is invalid

    //     tokens.push_back(token) ## save the token
    //     start = end + 1 ## move past this token

    // while (end > 0) ## continue loop condition

    do {
        end = csvLine.find(separator, start);
        if (start == csvLine.length() || start == end) break;
        else if (end >= 0) token = csvLine.substr(start, end - start);
        else token = csvLine.substr(start, csvLine.length() - start);
        tokens.push_back(token);
        start = end + 1;
    } while (end != std::string::npos);
    // std::string::npos = “not a valid position” = usually means search failed.

    return tokens;
}

int main() {
    // std::string s = "2020/03/17 17:01:24.884492,ETH/BTC,bid,0.02186251,0.0091";
    // std::vector<std::string> tokens = tokenise(s, ',');
    // for (auto& t : tokens) {
    //     std::cout << t << std::endl;
    // }

    std::ifstream csvFile{ "20200317.csv" };
    std::string line;
    std::vector<std::string> tokens;

    if (csvFile.is_open()) {
        std::cout << "File opened " << std::endl;

        while (std::getline(csvFile, line)) {   // get line from csvFile and store in line
            std::cout << "Read line: " << line << std::endl;
            tokens = tokenise(line, ',');

            // Basic check: correct number of tokens
            if (tokens.size() != 5) {
                std::cout << "Bad line " << std::endl;
                continue;
            }
            // Convert tokens into correct data types
            try {
                std::string timestamp = tokens[0];
                std::string product = tokens[1];
                OrderBookType orderType = tokens[2] == "bid" ? OrderBookType::bid : OrderBookType::ask;
                double price = std::stod(tokens[3]);
                double amount = std::stod(tokens[4]);
                std::cout << price << " : " << amount << std::endl;
            }
            catch (std::exception& e) {
                std::cout << e.what() << std::endl;
                std::cout << "OrderBookType: " << tokens[2] << std::endl;
                std::cout << "Bad float! " << tokens[3] << std::endl;
                std::cout << "Bad float! " << tokens[4] << std::endl;
                continue;
            }


        }
        csvFile.close();
    }
    else {
        std::cout << "File not opened " << std::endl;
    }

    return 0;
}


