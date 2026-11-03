#ifndef STUDENTPTA_CONSOLE_H
#define STUDENTPTA_CONSOLE_H

#include <string>

// Terminal input/output helpers. Every reader re-prompts until it gets a valid
// value, so a stray letter can no longer leave std::cin in a failed state.
// If input ends (Ctrl-D) they exit the program cleanly.

void clearScreen();
void printError(const std::string& message);

long long readLongLong(const std::string& prompt, long long min, long long max);
int readInt(const std::string& prompt, int min, int max);
std::string readWord(const std::string& prompt);
// Reads one of the characters in `allowed` (case-insensitive), returned upper-cased.
char readChoice(const std::string& prompt, const std::string& allowed);

// Case-insensitive ASCII string comparison.
bool equalsIgnoreCase(const std::string& a, const std::string& b);

#endif
