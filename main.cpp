#include <algorithm>
#include <cmath>
#include <iostream>
#include <string>
#include <vector>

const int DEFAULT_BASE = 2;

void ConvertStringsToIntegers(std::vector<int> &integerVector, const std::vector<std::string> &stringVector);
std::vector<int> SplitStringToDigits(const std::string &inputString, int BASE);
std::vector<std::string> SplitStringIntoParts(const std::string &stringToSplit, int BASE);

class BigInt {
private:
    const int BASE;
    const int TEN_TO_BASE;
    std::vector<int> digits;
    bool sign;

public:
    BigInt(int BASE = DEFAULT_BASE)
        : BASE{BASE}, TEN_TO_BASE{static_cast<int>(std::pow(10, BASE))} {
        this->digits.push_back(0);
        this->sign = true;
    }

    BigInt(std::string inputString, int BASE = DEFAULT_BASE)
        : BASE{BASE}, TEN_TO_BASE{static_cast<int>(std::pow(10, BASE))} {
        this->digits = SplitStringToDigits(inputString, BASE);
    }

    std::vector<int> getDigits() {
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

    void operator+=(long long int number) {
        int numDigits = static_cast<int>(std::floor(std::log(number) / std::log(this->TEN_TO_BASE))) + 1;

        int sizeDifference = numDigits - this->digits[0];
        if (sizeDifference > 0) {
            this->digits.resize(this->digits.size() + sizeDifference, 0);
            this->digits[0] = numDigits;
        }

        int baseMultiplier = this->TEN_TO_BASE;
        for (int digitIndex = 1; digitIndex <= numDigits; digitIndex++) { // Start with i = 1, because
            this->digits[digitIndex] += number % baseMultiplier;          // first element is size of vector

            if (this->digits[digitIndex] >= this->TEN_TO_BASE) {
                if (this->digits[0] == digitIndex) {
                    this->digits.push_back(0);
                    this->digits[0] += 1;
                }
                this->digits[digitIndex + 1] += this->digits[digitIndex] / this->TEN_TO_BASE;
                this->digits[digitIndex] %= this->TEN_TO_BASE;
            }

            number /= baseMultiplier;
            baseMultiplier *= baseMultiplier;
        }
    }

    BigInt operator+(long long number) {
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
            if (this->digits[0] == digitIndex && sumOfDigits >= bigInteger.TEN_TO_BASE) {
                this->digits.push_back(0);
                this->digits[0] += 1;
            }

            this->digits[digitIndex + 1] += sumOfDigits / this->TEN_TO_BASE;
            this->digits[digitIndex] = sumOfDigits % this->TEN_TO_BASE;
        }
    }

    BigInt operator+(const BigInt &bigInteger) {
        BigInt resultInteger = *this;
        resultInteger += bigInteger;
        return resultInteger;
    }

    void operator*=(long long int number) {
        int numberDigitPosition = 0;
        BigInt resultInteger{"0", this->BASE};
        while (number > 0) {
            BigInt mulBigInt = *this;

            for (int i = 1; i <= mulBigInt.digits[0]; i++) {
                if (mulBigInt.digits[i] >= this->TEN_TO_BASE) {
                    if (mulBigInt.digits[0] == i) {
                        mulBigInt.digits.push_back(0);
                        ++mulBigInt.digits[0];
                    }

                    mulBigInt.digits[i + 1] += mulBigInt.digits[i] / this->TEN_TO_BASE;
                    mulBigInt.digits[i] %= this->TEN_TO_BASE;
                }
            }

            int multiplier = (number % this->TEN_TO_BASE);
            for (int i = 1; i <= mulBigInt.digits[0]; i++) {
                mulBigInt.digits[i] *= multiplier;
            }

            std::vector<int> zeros(numberDigitPosition, 0);
            mulBigInt.digits.insert(mulBigInt.digits.begin() + 1, zeros.begin(), zeros.end());
            mulBigInt.digits[0] += numberDigitPosition;

            for (int i = 1; i <= mulBigInt.digits[0]; i++) {
                if (mulBigInt.digits[i] >= this->TEN_TO_BASE) {
                    if (mulBigInt.digits[0] == i) {
                        mulBigInt.digits.push_back(0);
                        ++mulBigInt.digits[0];
                    }

                    mulBigInt.digits[i + 1] += mulBigInt.digits[i] / this->TEN_TO_BASE;
                    mulBigInt.digits[i] %= this->TEN_TO_BASE;
                }
            }

            resultInteger += mulBigInt;
            number /= this->TEN_TO_BASE;
            ++numberDigitPosition;
        }

        this->digits = resultInteger.digits;
    }

    BigInt operator*(long long number) {
        BigInt resultInteger = *this;
        resultInteger *= number;

        return resultInteger;
    }

    // BigInt(int base = DEFAULT_BASE, int max_digits = DEFAULT_MAX_DIGITS, long long int inputString)
    //     : BASE{base}, MAX_DIGITS{max_digits} {}
};

int main() {
    BigInt ar{"1", 9};
    // BigInt ar1{std::string("9990"), 2};

    for (int i = 1; i <= 100; i++) {
        ar *= i;
    }

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