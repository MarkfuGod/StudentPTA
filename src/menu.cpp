#include "menu.h"

#include <cstdio>
#include <iostream>
#include <string>

#include "banner.h"
#include "console.h"

namespace {

const char* const kRule = "\033[36m+----------------+------+------+------+-------+------+--------+\033[0m\n";

void printTableHeader() {
    std::printf("%s", kRule);
    std::printf("| \033[37mStudent Number\033[0m | \033[37mPnum\033[0m | \033[37mTime\033[0m | \033[37mPass\033[0m "
                "| \033[37mWrong\033[0m | \033[37mDup%%\033[0m | \033[37mResult\033[0m |\n");
    std::printf("%s", kRule);
}

void printRow(const Record& r) {
    std::printf("|  \033[01;33m%-12lld\033[0m  |\033[01;33m%-6c\033[0m|\033[01;33m%-6d\033[0m|\033[01;33m%-6d\033[0m"
                "|\033[01;33m%-7d\033[0m|\033[01;33m%-6d\033[0m|\033[01;33m%8.2f\033[0m|\n",
                r.studentNumber, r.problem, r.time, r.passed ? 1 : 0, r.wrongTimes,
                r.duplicateRate, r.result);
    std::printf("%s", kRule);
}

const std::string kAsk = "\033[01;33;46m";
const std::string kEnd = "\033[0m ";

}  // namespace

void login() {
    while (readWord("\033[01;04;32m Please Input the Account Name:\033[0m ") != kAccountName) {
    }
    std::cout << "SUCCESS !" << std::endl;
}

void inputRecords(RecordList& list) {
    clearScreen();
    while (true) {
        Record r = {};
        r.studentNumber = readLongLong(kAsk + "Student Number (12 digits):" + kEnd,
                                       kMinStudentNumber, kMaxStudentNumber);
        r.problem = readChoice(kAsk + "Problem Number [A-E]:" + kEnd, "ABCDE");
        r.time = readInt(kAsk + "Time used (seconds):" + kEnd, 0, 1000000);
        r.passed = readChoice(kAsk + "Passed? [1/0]:" + kEnd, "10") == '1';
        r.wrongTimes = readInt(kAsk + "Wrong Times:" + kEnd, 0, 100000);
        r.duplicateRate = readInt(kAsk + "Duplicate Rate (0-100):" + kEnd, 0, 100);
        r.result = 0.0;
        append(list, r);

        if (readChoice(kAsk + "Continue entering? [y/n]" + kEnd, "yn") == 'N') {
            clearScreen();
            return;
        }
        clearScreen();
    }
}

void showRecords(const RecordList& list) {
    printTableHeader();
    for (const RecordNode* p = list.head; p; p = p->next) printRow(p->data);
}

void calculateResults(RecordList& list, const ScoreConfig& config) {
    while (true) {
        long long id = readLongLong(
            "\033[01;32mStudent Number to calculate\033[0m (\033[01;33m0\033[0m to go back): ",
            0, kMaxStudentNumber);
        if (id == 0) return;

        double total = 0.0;
        int found = 0;
        for (RecordNode* p = list.head; p; p = p->next) {
            if (p->data.studentNumber != id) continue;
            p->data.result = scoreRecord(p->data, config);
            if (found++ == 0) printTableHeader();
            printRow(p->data);
            total += p->data.result;
        }

        if (found == 0) {
            printError("No such student found, please type in the right number");
            continue;
        }
        std::printf("\033[01;32m Total: %.2f / 100\033[0m  (%d problem record%s)\n",
                    total, found, found == 1 ? "" : "s");

        if (readChoice("Calculate another student? [y/n] ", "yn") == 'N') return;
    }
}

void editScores(ScoreConfig& config) {
    struct Field {
        const char* item;
        const char* range;
        int* points;
    };
    const Field fields[] = {
        {"Time", "<= 500s", &config.time[0]},
        {"Time", "<= 1000s", &config.time[1]},
        {"Time", "<= 3000s", &config.time[2]},
        {"Time", "> 3000s", &config.time[3]},
        {"Wrong", "0 times", &config.wrong[0]},
        {"Wrong", "1-3 times", &config.wrong[1]},
        {"Wrong", "> 3 times", &config.wrong[2]},
        {"Duplicate", "<= 20%", &config.duplicate[0]},
        {"Duplicate", "<= 35%", &config.duplicate[1]},
        {"Duplicate", "<= 50%", &config.duplicate[2]},
    };
    const int count = sizeof(fields) / sizeof(fields[0]);

    clearScreen();
    while (true) {
        std::printf("\033[36m+----+-----------+------------+--------+\033[0m\n");
        std::printf("| \033[37m#\033[0m  | \033[37mItem\033[0m      | \033[37mRange\033[0m      | \033[37mPoints\033[0m |\n");
        std::printf("\033[36m+----+-----------+------------+--------+\033[0m\n");
        for (int i = 0; i < count; ++i)
            std::printf("| %-2d | %-9s | %-10s | \033[01;33m%-6d\033[0m |\n", i + 1, fields[i].item,
                        fields[i].range, *fields[i].points);
        std::printf("|    | %-9s | %-10s | \033[35m%-6d\033[0m |  (fixed)\n", "Pass", "passed", kPassScore);
        std::printf("\033[36m+----+-----------+------------+--------+\033[0m\n");
        std::printf("Over %d%% duplicate or not passed scores 0. Problem weights A-E: "
                    "0.10 0.15 0.20 0.25 0.30\n", kMaxDuplicateRate);

        int pick = readInt("\033[01;32mWhich row to edit?\033[0m (\033[01;31m0 to go back\033[0m): ", 0, count);
        if (pick == 0) {
            clearScreen();
            return;
        }
        const Field& f = fields[pick - 1];
        *f.points = readInt(std::string("New points for ") + f.item + " " + f.range + " (0-" +
                                std::to_string(kMaxTierScore) + "): ",
                            0, kMaxTierScore);
        clearScreen();
    }
}

void runPlain(RecordList& list, ScoreConfig& config) {
    printZZU();
    login();
    printWelcome();

    bool running = true;
    while (running) {
        printMainMenu();
        switch (readInt("\033[01;34m Please Input Your Choice:\033[0m ", 1, 5)) {
            case 1:
                editScores(config);
                break;
            case 2:
                clearScreen();
                showRecords(list);
                break;
            case 3:
                calculateResults(list, config);
                clearScreen();
                break;
            case 4:
                inputRecords(list);
                break;
            case 5:
                running = false;
                break;
        }
    }

    clearScreen();
    printGoodBye();
    printPTA();
}
