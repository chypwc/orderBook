/*
Dataset: https://www.kaggle.com/datasets/bhadramohit/world-bank-dataset?resource=download

*/
#include <iostream>
#include <string>
#include <vector>
#include <fstream>

std::vector<std::string> tokenise(const std::string& csvLine, char separator);

struct Country {
    std::string country;
    int    year;
    double gdp;
    double population;
    double lifeExpectancy;
    double unemploymentRate;
    double co2Emissions;
    double accessToElectricity;

    Country(std::string c, int y, double g, double p,
        double life, double unemp, double co2, double electric)
        : country(std::move(c)), year(y), gdp(g), population(p),
        lifeExpectancy(life), unemploymentRate(unemp),
        co2Emissions(co2), accessToElectricity(electric) {
    }
};

int main() {
    std::ifstream csvFile{ "./world_bank_dataset.csv" };
    std::string line;
    std::vector<std::string> tokens;
    std::vector<Country> countries;
    std::size_t lineNo{ 0 }; // track error line 


    if (csvFile.is_open()) {
        std::cout << "File opened successfully" << std::endl;
        // skip header
        if (std::getline(csvFile, line)) {
            std::cout << "Header: " << line << std::endl;
            ++lineNo;
        }

        while (std::getline(csvFile, line)) {
            ++lineNo;

            tokens = tokenise(line, ',');
            // Basic check the number of fields
            if (tokens.size() != 8) {
                std::cout << "Bad line " << lineNo << std::endl;
                continue;
            }
            // data conversion
            try {
                const std::string& name = tokens[0];
                int    year = std::stoi(tokens[1]);
                double gdp = std::stod(tokens[2]);
                double population = std::stod(tokens[3]);
                double lifeExpectancy = std::stod(tokens[4]);
                double unemploymentRate = std::stod(tokens[5]);
                double co2Emissions = std::stod(tokens[6]);
                double accessElectricity = std::stod(tokens[7]);

                countries.emplace_back(name, year, gdp, population,
                    lifeExpectancy, unemploymentRate,
                    co2Emissions, accessElectricity);
            }
            catch (const std::exception& e) {
                std::cerr << "Error parsing data: " << e.what() << std::endl;
                continue;
            }
        }
        csvFile.close();
    }
    else {
        std::cerr << "Error opening file" << std::endl;
    }

    std::cout << "Number of valid entries: " << countries.size() << std::endl;

    if (!countries.empty()) {
        double sumLife = 0.0;
        for (const auto& c : countries) {
            sumLife += c.lifeExpectancy;
        }
        std::cout << "Life expectancy average = " << (sumLife / countries.size()) << std::endl;
    }

    return 0;
}

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