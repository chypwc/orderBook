#include <string>
#include <iostream>
#include <fstream>

std::string line;
int main() {
std:ifstream csvFile{ “test.csv” };
    if (csvFile.is_open()) {
        std::cout << “File open” << std::endl;
        std::getline(csvFile, line);
        std::cout << line << std::endl;
        csvFile.close();
    }
}