#include <algorithm>
#include <cmath>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

const int DEFAULT_BASE = 2;

void ConvertStringsToIntegers(std::vector<uint_fast64_t> &integerVector, const std::vector<std::string> &stringVector);
std::vector<uint_fast64_t> SplitStringToDigits(const std::string &inputString, int BASE);
std::vector<std::string> SplitStringIntoParts(const std::string &stringToSplit, int BASE);

class BigInt {
private:
    const int BASE;
    const uint_fast64_t TEN_TO_BASE;
    std::vector<uint_fast64_t> digits;

    bool isNegative;

    inline void NormalizeDigit(int i) {
        if (this->digits[i] >= this->TEN_TO_BASE) {
            if (this->digits[0] == i) {
                this->digits.push_back(0);
                ++this->digits[0];
            }

            this->digits[i + 1] += this->digits[i] / this->TEN_TO_BASE;
            this->digits[i] %= this->TEN_TO_BASE;
        }
    }

    static uint_fast64_t computeTenToBase(int base) {
        uint_fast64_t value = 1;
        for (int i = 0; i < base; ++i) {
            value *= 10;
        }
        return value;
    }

    bool isNumberBiggerThanBigInt(uint_fast64_t number) {
        int numDigits = 0;
        uint_fast64_t tempNumber = number;
        while (tempNumber > 0) {
            tempNumber /= this->TEN_TO_BASE;
            numDigits++;
        }

        if (this->digits[0] < numDigits) {
            return true;
        }

        if (this->digits[0] > numDigits) {
            return false;
        }

        int baseMultiplier = std::pow(this->TEN_TO_BASE, numDigits - 1);

        for (int i = this->digits[0]; i >= 1; i--) {
            if (number / baseMultiplier > this->digits[i]) {
                return true;
            }

            number /= this->TEN_TO_BASE;
            baseMultiplier /= this->TEN_TO_BASE;
        }

        return false;
    }

public:
    BigInt(int BASE = DEFAULT_BASE)
        : BASE{BASE}, TEN_TO_BASE{computeTenToBase(BASE)} {
        this->digits.push_back(0);
        this->isNegative = true;
    }

    BigInt(std::string inputString, int BASE = DEFAULT_BASE)
        : BASE{BASE}, TEN_TO_BASE{computeTenToBase(BASE)} {
        this->digits = SplitStringToDigits(inputString, BASE);
    }

    std::vector<uint_fast64_t> getDigits() {
        return this->digits;
    }

    std::string getStringDigits() {
        std::string digitsString{};
        for (int i = this->digits[0]; i >= 1; i--) {
            std::string digitString = std::to_string(this->digits[i]);

            while (digitString.size() < this->BASE) {
                if (i == this->digits[0]) {
                    break;
                }
                digitString = "0" + digitString;
            }
            digitsString += digitString;
        }

        return digitsString;
    }

    BigInt &operator=(const BigInt &bigInteger) {
        if (this == &bigInteger) {
            return *this;
        }

        this->digits = bigInteger.digits;
        return *this;
    }

    void operator+=(uint_fast64_t number) {
        int numDigits = 0;
        uint_fast64_t tempNumber = number;
        while (tempNumber > 0) {
            tempNumber /= this->TEN_TO_BASE;
            numDigits++;
        }

        int sizeDifference = numDigits - this->digits[0];
        if (sizeDifference > 0) {
            this->digits.resize(this->digits.size() + sizeDifference, 0);
            this->digits[0] = numDigits;
        }

        int baseMultiplier = this->TEN_TO_BASE;
        for (int digitIndex = 1; digitIndex <= numDigits; digitIndex++) { // Start with i = 1, because
            this->digits[digitIndex] += number % baseMultiplier;          // first element is size of vector

            this->NormalizeDigit(digitIndex);

            number /= baseMultiplier;
            baseMultiplier *= baseMultiplier;
        }
    }

    BigInt operator+(uint_fast64_t number) {
        BigInt resultInteger = *this;
        resultInteger += number;

        return resultInteger;
    }

    void operator+=(const BigInt &bigInteger) {
        int sizeDifference = bigInteger.digits[0] - this->digits[0];
        if (sizeDifference > 0) {
            this->digits.resize(this->digits.size() + sizeDifference, 0);
            this->digits[0] = bigInteger.digits[0];
        }

        for (int digitIndex = 1; digitIndex <= bigInteger.digits[0]; digitIndex++) {
            int sumOfDigits = (this->digits[digitIndex] + bigInteger.digits[digitIndex]);
            this->digits[digitIndex] = sumOfDigits;

            this->NormalizeDigit(digitIndex);
        }
    }

    BigInt operator+(const BigInt &bigInteger) {
        BigInt resultInteger = *this;
        resultInteger += bigInteger;
        return resultInteger;
    }

    void operator-=(uint_fast64_t number) {
        int numDigits = 0;
        uint_fast64_t tempNumber = number;
        while (tempNumber > 0) {
            tempNumber /= this->TEN_TO_BASE;
            numDigits++;
        }

        // int sizeDifference = numDigits - this->digits[0];
        // if (sizeDifference > 0) {
        //     this->digits.resize(this->digits.size() + sizeDifference, 0);
        //     this->digits[0] = numDigits;
        // }

        if (this->isNumberBiggerThanBigInt(number)) {
            for (int i = 1; i <= this->digits[0]; i++) {
                number -= this->digits[i] * std::pow(this->TEN_TO_BASE, i - 1);
                this->digits[i] = 0;
                --this->digits[0];
            }
            this->isNegative = true;
            *this += number;
            return;
        }

        int baseMultiplier = this->TEN_TO_BASE;
        for (int digitIndex = 1; digitIndex <= numDigits; digitIndex++) {
            if (this->digits[digitIndex] < number % baseMultiplier) {
                this->digits[digitIndex + 1] -= 1;
                this->digits[digitIndex] += this->TEN_TO_BASE;
            }

            this->digits[digitIndex] -= number % baseMultiplier;

            this->NormalizeDigit(digitIndex);

            number /= baseMultiplier;
            baseMultiplier *= baseMultiplier;
        }

        while (this->digits[0] > 0 && this->digits.back() == 0) {
            this->digits.pop_back();
            --this->digits[0];
        }
    }

    void operator*=(uint_fast64_t number) {
        int numberDigitPosition = 0;
        BigInt resultInteger{"0", this->BASE};
        while (number > 0) {
            BigInt tempBigInteger = *this;

            int multiplier = (number % this->TEN_TO_BASE);
            for (int i = 1; i <= tempBigInteger.digits[0]; i++) {
                tempBigInteger.digits[i] *= multiplier;
            }

            tempBigInteger.digits.insert(tempBigInteger.digits.begin() + 1, numberDigitPosition, 0);
            tempBigInteger.digits[0] += numberDigitPosition;

            for (int i = 1; i <= tempBigInteger.digits[0]; i++) {
                tempBigInteger.NormalizeDigit(i);
            }

            resultInteger += tempBigInteger;
            number /= this->TEN_TO_BASE;
            ++numberDigitPosition;
        }

        this->digits = std::move(resultInteger.digits);
    }

    BigInt operator*(uint_fast64_t number) {
        BigInt resultInteger = *this;
        resultInteger *= number;

        return resultInteger;
    }

    void operator*=(const BigInt &mulBigInteger) {
        int numberDigitPosition = 0;
        BigInt resultInteger{"0", this->BASE};

        for (int i = 1; i <= mulBigInteger.digits[0]; i++) {
            BigInt tempBigInteger = *this;

            int multiplier = mulBigInteger.digits[i];
            for (int j = 1; j <= tempBigInteger.digits[0]; j++) {
                tempBigInteger.digits[j] *= multiplier;
            }

            tempBigInteger.digits.insert(tempBigInteger.digits.begin() + 1, numberDigitPosition, 0);
            tempBigInteger.digits[0] += numberDigitPosition;

            tempBigInteger.NormalizeDigit(i);

            resultInteger += tempBigInteger;
            ++numberDigitPosition;
        }

        this->digits = resultInteger.digits;
    }

    BigInt operator*(const BigInt &bigInteger) {
        BigInt resultInteger = *this;
        resultInteger *= bigInteger;

        return resultInteger;
    }

    BigInt &operator++() {
        if (this->digits[0] == 0) {
            this->digits.resize(this->digits.size() + 1, 0);
            ++this->digits[0];
        }

        if (++this->digits[1] >= this->TEN_TO_BASE) {
            if (this->digits[0] == 1) {
                this->digits.resize(this->digits.size() + 1, 0);
                ++this->digits[0];
            }
            ++this->digits[2];
            this->digits[1] = 0;
        }

        return *this;
    }

    BigInt operator++(int) {
        BigInt temp = *this;
        ++(*this);
        return temp;
    }

    bool operator==(const BigInt &bigInteger) {
        return this->digits == bigInteger.digits;
    }
};

// BigInt(int base = DEFAULT_BASE, int max_digits = DEFAULT_MAX_DIGITS, uint_fast64_t inputString)
//     : BASE{base}, MAX_DIGITS{max_digits} {}
int main() {
    BigInt ar{"1", 9};

    // std::cout << ar.getStringDigits() << "\n";

    for (int i = 2; i <= 300000; i++) {
        ar *= i;
    }

    std::ofstream outputFile;
    outputFile.open("big_factor");

    if (outputFile.is_open()) {
        outputFile << ar.getStringDigits() << std::endl;
    }

    outputFile.close();

    // for (const int &digit : ar.getDigits()) {
    //     std::cout << digit << " ";
    // }

    // do {
    //     std::cout << '\n'
    //               << "Press the Enter key to continue.";
    // } while (std::cin.get() != '\n');

    return 0;
}

std::vector<uint_fast64_t> SplitStringToDigits(const std::string &inputString, int BASE) {
    std::vector<std::string> splitParts;

    splitParts = SplitStringIntoParts(inputString, BASE);

    std::reverse(splitParts.begin(), splitParts.end());

    std::vector<uint_fast64_t> digits;
    digits.push_back(splitParts.size());

    ConvertStringsToIntegers(digits, splitParts);

    return digits;
}

void ConvertStringsToIntegers(std::vector<uint_fast64_t> &integerVector, const std::vector<std::string> &stringVector) {
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