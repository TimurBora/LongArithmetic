#include <cmath>
#include <iostream>
#include <string>
#include <vector>

const int DEFAULT_BASE_BY_TEN = 2;
const int DEFAULT_MAX_DIGITS = 1000;

std::vector<int> split_string_to_digits(std::string value) {
    std::vector<std::string> strings_digits;

    if (value.size() % 2 != 0) {
        int remainder_digits = value.size() % 2;
        std::string remainder_substring = value.substr(0, remainder_digits);
        strings_digits.insert(strings_digits.begin(), remainder_substring);
        value.erase(0, remainder_digits);
    }

    int digits_size = value.size() / 2 + value.size() % 2;

    for (int i = 0; i < digits_size; i++) {
        std::string substring = value.substr(i * 2, 2);
        strings_digits.insert(strings_digits.begin(), substring);
    }

    std::vector<int> digits;
    digits.push_back(digits_size);

    for (const std::string &string : strings_digits) {
        digits.push_back(std::stoi(string));
    }

    return digits;
}

class LongArithmeticInt {
private:
    const int BASE;
    const int MAX_DIGITS;
    std::vector<int> digits;

    std::vector<std::string> split_string_to_digits1(std::string value) {
        std::vector<std::string> strings_digits;

        if (value.size() % this->BASE != 0) {
            int remainder_digits = value.size() % this->BASE;
            std::string remainder_substring = value.substr(0, remainder_digits);
            strings_digits.insert(strings_digits.begin(), remainder_substring);
            value.erase(0, remainder_digits);
        }

        int digits_size = value.size() / this->BASE + value.size() % this->BASE;

        for (int i = 0; i < digits_size; i++) {
            std::string substring = value.substr(i * this->BASE, this->BASE);
            strings_digits.insert(strings_digits.begin(), substring);
        }
        return strings_digits;
    }

public:
    // LongArithmeticInt(int base = DEFAULT_BASE_BY_TEN, int max_digits = DEFAULT_MAX_DIGITS)
    //     : BASE{base}, MAX_DIGITS{max_digits}, digits() {}

    // LongArithmeticInt(int base = DEFAULT_BASE_BY_TEN, int max_digits = DEFAULT_MAX_DIGITS, std::string value)
    //     : BASE{base}, MAX_DIGITS{max_digits} {}

    // LongArithmeticInt(int base = DEFAULT_BASE_BY_TEN, int max_digits = DEFAULT_MAX_DIGITS, long long int value)
    //     : BASE{base}, MAX_DIGITS{max_digits} {}
};

int main() {
    std::vector<int> digits = split_string_to_digits("12345");
    for (const int &value : digits) {
        std::cout << value << " ";
    }
    do {
        std::cout << '\n'
                  << "Press the Enter key to continue.";
    } while (std::cin.get() != '\n');

    return 0;
}