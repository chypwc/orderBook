#include <iostream>
#include <string>

int main() {
    std::string s = "Naxwell";
    std::cout << "length of s " << s.length() << std::endl; // .length is not static, stateful

    std::cout << std::stod("55");  // stod: static function, no state

    return 0;
}