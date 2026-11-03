#include "console.h"

#include <cctype>
#include <cstdlib>
#include <iostream>
#include <limits>

namespace {

void exitOnEof() {
    if (std::cin.eof()) {
        std::cout << "\n";
        std::exit(0);
    }
}

void discardLine() {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

}  // namespace

void clearScreen() {
    std::cout << "\033[2J\033[H" << std::flush;  // ANSI: clear screen, cursor home
}

void printError(const std::string& message) {
    std::cout << "\033[01;33;41m " << message << " \033[0m" << std::endl;
}

long long readLongLong(const std::string& prompt, long long min, long long max) {
    while (true) {
        std::cout << prompt;
        long long value;
        if (std::cin >> value) {
            if (value >= min && value <= max) return value;
            printError("Out of range, please enter a value between " +
                       std::to_string(min) + " and " + std::to_string(max));
            continue;
        }
        exitOnEof();
        discardLine();
        printError("Not a number, please try again");
    }
}

int readInt(const std::string& prompt, int min, int max) {
    return static_cast<int>(readLongLong(prompt, min, max));
}

std::string readWord(const std::string& prompt) {
    std::cout << prompt;
    std::string word;
    if (!(std::cin >> word)) exitOnEof();
    return word;
}

char readChoice(const std::string& prompt, const std::string& allowed) {
    while (true) {
        std::string word = readWord(prompt);
        if (word.size() == 1) {
            char c = static_cast<char>(std::toupper(static_cast<unsigned char>(word[0])));
            for (char a : allowed)
                if (std::toupper(static_cast<unsigned char>(a)) == c) return c;
        }
        printError("Please enter one of: " + allowed);
    }
}

bool equalsIgnoreCase(const std::string& a, const std::string& b) {
    if (a.size() != b.size()) return false;
    for (size_t i = 0; i < a.size(); ++i)
        if (std::tolower(static_cast<unsigned char>(a[i])) !=
            std::tolower(static_cast<unsigned char>(b[i])))
            return false;
    return true;
}
