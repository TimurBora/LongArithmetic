#include <algorithm>
#include <cmath>
#include <iostream>
#include <string>
#include <vector>

const int DEFAULT_BASE = 2;
const int DEFAULT_MAX_DIGITS = 1000;

// BASE = 3
// 12 345
// remainderLenght = 2
// numParts = 3

std::vector<int>
SplitStringToDigits(std::string inputString, int BASE) {
    std::vector<std::string> splitParts;

    int remainderLength = inputString.size() % BASE;
    int numParts = inputString.size() / BASE + (remainderLength != 0);

    if (remainderLength != 0) {
        std::string remainderPart = inputString.substr(0, remainderLength);
        splitParts.push_back(remainderPart);
    }

    for (int i = 0; i < numParts - 1; i++) {
        std::string part = inputString.substr(i * BASE + remainderLength, BASE);
        splitParts.push_back(part);
    }

    std::reverse(splitParts.begin(), splitParts.end());

    std::vector<int> digits;
    digits.push_back(numParts);

    for (const std::string &string : splitParts) {
        digits.push_back(std::stoi(string));
    }

    return digits;
}

class LongArithmeticInt {
private:
    const int BASE;
    const int MAX_DIGITS;
    std::vector<int> digits;

public:
    LongArithmeticInt(int BASE = DEFAULT_BASE, int MAX_DIGITS = DEFAULT_MAX_DIGITS)
        : BASE{BASE}, MAX_DIGITS{MAX_DIGITS}, digits() {}

    LongArithmeticInt(std::string inputString, int BASE = DEFAULT_BASE, int MAX_DIGITS = DEFAULT_MAX_DIGITS)
        : BASE{BASE}, MAX_DIGITS{MAX_DIGITS} {
        this->digits = SplitStringToDigits(inputString, BASE);
    }

    std::vector<int> getDigits() {
        return this->digits;
    }

    // LongArithmeticInt(int base = DEFAULT_BASE_BY_TEN, int max_digits = DEFAULT_MAX_DIGITS, long long int inputString)
    //     : BASE{base}, MAX_DIGITS{max_digits} {}
};

int main() {
    LongArithmeticInt ar{std::string("12345"), 4};

    for (const int &digit : ar.getDigits()) {
        std::cout << digit << " ";
    }
    do {
        std::cout << '\n'
                  << "Press the Enter key to continue.";
    } while (std::cin.get() != '\n');

    return 0;
}