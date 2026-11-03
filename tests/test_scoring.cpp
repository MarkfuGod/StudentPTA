// Checks the refactored scoring against the output of the original program
// (tests/golden_scores.csv, produced by tests/legacy_dump.cpp in commit deae584,
// before the rewrite), plus a few direct checks of the tier boundaries.
//
//   make test
#include <cmath>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include "scoring.h"

static int failures = 0;

static void expectEq(int got, int want, const char* what) {
    if (got != want) {
        std::cerr << "FAIL " << what << ": got " << got << ", want " << want << "\n";
        ++failures;
    }
}

static void checkTiers() {
    expectEq(timeTier(0), 0, "time 0");
    expectEq(timeTier(500), 0, "time 500");
    expectEq(timeTier(501), 1, "time 501");
    expectEq(timeTier(1000), 1, "time 1000");
    expectEq(timeTier(3000), 2, "time 3000");
    expectEq(timeTier(3001), 3, "time 3001");
    expectEq(wrongTier(0), 0, "wrong 0");
    expectEq(wrongTier(1), 1, "wrong 1");
    expectEq(wrongTier(3), 1, "wrong 3");
    expectEq(wrongTier(4), 2, "wrong 4");
    expectEq(duplicateTier(20), 0, "dup 20");
    expectEq(duplicateTier(21), 1, "dup 21");
    expectEq(duplicateTier(35), 1, "dup 35");
    expectEq(duplicateTier(36), 2, "dup 36");
    expectEq(duplicateTier(50), 2, "dup 50");
}

static void checkFullMarks() {
    ScoreConfig c = defaultScoreConfig();
    double total = 0;
    for (int i = 0; i < kProblemCount; ++i) {
        Record r = {1, static_cast<char>('A' + i), 0, true, 0, 0, 0};
        total += scoreRecord(r, c);
    }
    if (std::fabs(total - 100.0) > 1e-9) {
        std::cerr << "FAIL full marks: got " << total << ", want 100\n";
        ++failures;
    }
}

static int checkGolden(const char* path) {
    std::ifstream in(path);
    if (!in) {
        std::cerr << "cannot open " << path << "\n";
        ++failures;
        return 0;
    }
    ScoreConfig c = defaultScoreConfig();
    std::string line;
    std::getline(in, line);  // header
    int rows = 0;
    while (std::getline(in, line)) {
        Record r = {};
        int passed = 0;
        double expected = 0;
        char comma;
        std::istringstream ss(line);
        ss >> r.problem >> comma >> r.time >> comma >> passed >> comma
           >> r.wrongTimes >> comma >> r.duplicateRate >> comma >> expected;
        r.passed = passed != 0;
        double got = scoreRecord(r, c);
        if (std::fabs(got - expected) > 1e-6) {
            std::cerr << "FAIL golden row '" << line << "': got " << got << "\n";
            ++failures;
        }
        ++rows;
    }
    return rows;
}

int main(int argc, char** argv) {
    const char* golden = argc > 1 ? argv[1] : "tests/golden_scores.csv";
    checkTiers();
    checkFullMarks();
    int rows = checkGolden(golden);
    if (failures) {
        std::cerr << failures << " failure(s)\n";
        return 1;
    }
    std::cout << "ok: tier boundaries, full marks, " << rows << " golden rows\n";
    return 0;
}
