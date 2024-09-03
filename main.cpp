#include <algorithm>
#include <cmath>
#include <iostream>
#include <string>
#include <vector>

using std::string;
using std::vector;

const int DEFAULT_BASE = 2;
const int DEFAULT_MAX_DIGITS = 1000;

void ConvertStringsToIntegers(std::vector<int> &integerVector, const std::vector<std::string> &stringVector);
std::vector<int> SplitStringToDigits(const std::string &inputString, int BASE);
std::vector<std::string> SplitStringIntoParts(const std::string &stringToSplit, int BASE);

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

    void Addition(long long int number) {
        int numDigits = 0;
        const int TEN_TO_BASE = std::pow(10, this->BASE);
        int baseMultiplier = TEN_TO_BASE;

        while (number / baseMultiplier >= 0) {
            numDigits += 1;

            if (number / baseMultiplier == 0) {
                break;
            }

            baseMultiplier *= baseMultiplier;
        }

        while (this->digits[0] < numDigits) {
            this->digits.push_back(0);
            this->digits[0] += 1;
        }

        baseMultiplier = TEN_TO_BASE;
        for (int digitIndex = 1; digitIndex <= numDigits; digitIndex++) { // Start with i = 1, because
            this->digits[digitIndex] += number % baseMultiplier;          // first element is size of vector

            if (this->digits[digitIndex] >= TEN_TO_BASE) {
                if (this->digits[0] == digitIndex) {
                    this->digits.push_back(0);
                    this->digits[0] += 1;
                }
                this->digits[digitIndex + 1] += this->digits[digitIndex] / TEN_TO_BASE;
                this->digits[digitIndex] %= TEN_TO_BASE;
            }

            number /= baseMultiplier;
            baseMultiplier *= baseMultiplier;
        }
    }

    void Multiply(long long int number) {
    }

    // LongArithmeticInt(int base = DEFAULT_BASE, int max_digits = DEFAULT_MAX_DIGITS, long long int inputString)
    //     : BASE{base}, MAX_DIGITS{max_digits} {}
};

int main() {
    LongArithmeticInt ar{std::string("0"), 2};
    ar.Addition(1234);

    for (const int &digit : ar.getDigits()) {
        std::cout << digit << " ";
    }
    do {
        std::cout << '\n'
                  << "Press the Enter key to continue.";
    } while (std::cin.get() != '\n');

    return 0;
}

std::vector<int> SplitStringToDigits(const std::string &inputString, int BASE) {
    std::vector<std::string> splitParts;

    splitParts = SplitStringIntoParts(inputString, BASE);

    std::reverse(splitParts.begin(), splitParts.end());

    std::vector<int> digits;
    digits.push_back(splitParts.size());

    ConvertStringsToIntegers(digits, splitParts);

    return digits;
}

void ConvertStringsToIntegers(std::vector<int> &integerVector, const std::vector<std::string> &stringVector) {
    for (const std::string &string : stringVector) {
        integerVector.push_back(std::stoi(string));
    }
}

std::vector<std::string> SplitStringIntoParts(const std::string &stringToSplit, int BASE) {
    std::vector<std::string> splitParts;
    int remainderLength = stringToSplit.size() % BASE;
    int numParts = stringToSplit.size() / BASE + (remainderLength != 0);

    if (remainderLength != 0) {
        std::string remainderPart = stringToSplit.substr(0, remainderLength);
        splitParts.push_back(remainderPart);
    }

    for (int i = 0; i < numParts - (remainderLength != 0); i++) {
        std::string part = stringToSplit.substr(i * BASE + remainderLength, BASE);
        splitParts.push_back(part);
    }

    return splitParts;
}